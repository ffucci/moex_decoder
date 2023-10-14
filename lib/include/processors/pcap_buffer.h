#include <atomic>
#include <cstddef>
#include <deque>
#include <fstream>
#include <iostream>

#include <mutex>
#include <optional>
#include <span>
#include <thread>
#include <vector>

#include "processors/pcap_types.h"
#include "processors/utility.h"

namespace task::processors::mt_buffer {

struct BufferedPackets {
  size_t start_packet_number{0};
  size_t number_packets{0};
  std::vector<std::vector<std::byte>> packets{};
};

class PCAPBuffer {
public:
  explicit PCAPBuffer(std::ifstream &file_handle, size_t file_size,
                      size_t offset)
      : file_handle_(file_handle), file_size_(file_size),
        current_offset_(offset) {}

  void start_buffering() {
    producer_thread_ = std::thread([this]() {
      std::cout << "# Start buffering " << std::endl;
      is_started_.store(true, std::memory_order_release);

      size_t packet_nr{1};
      while (current_offset_ < file_size_ &&
             is_started_.load(std::memory_order_acquire)) {
        size_t bytes_to_read = BATCH_SIZE;
        if (current_offset_ + BATCH_SIZE > file_size_) {
          bytes_to_read = file_size_ - current_offset_;
        }

        if constexpr (ENABLE_DEBUGGING) {
          std::cout << "Remaining bytes to process... " << std::dec
                    << bytes_to_read << std::endl;
        }

        if (!file_handle_.read((char *)local_buffer_.data(), bytes_to_read)) {
          throw std::runtime_error("Cannot read from the PCAP file.");
        }

        // buffer as many packets as possible that fit the BATCH SIZE
        BufferedPackets buffered_packets{};
        size_t offset{0};
        buffered_packets.start_packet_number = packet_nr;
        while (current_offset_ < file_size_ && offset < BATCH_SIZE) {
          pcap::types::pcaprec_hdr_s packet_header;
          std::span<std::byte> buffer_2{local_buffer_.data() + offset,
                                        sizeof(packet_header)};

          std::memcpy(&packet_header, buffer_2.data(), buffer_2.size());

          if constexpr (ENABLE_DEBUGGING) {
            std::cout << "usec: " << std::hex << packet_header.ts_usec
                      << std::endl;
            std::cout << "captured length: " << std::hex
                      << packet_header.captured_length << std::endl;
            std::cout << "original length: " << std::hex
                      << packet_header.orig_len << std::endl;
          }

          offset += sizeof(packet_header);

          if (offset + packet_header.captured_length >= BATCH_SIZE) {
            break;
          }

          if constexpr (ENABLE_DEBUGGING) {
            utility::hex_dump(local_buffer_.data() + offset,
                              packet_header.captured_length, std::cout);
          }
          buffered_packets.packets.emplace_back(
              local_buffer_.begin() + offset,
              local_buffer_.begin() + offset + packet_header.captured_length);
          buffered_packets.number_packets++;

          offset += packet_header.captured_length;
          current_offset_ +=
              sizeof(packet_header) + packet_header.captured_length;
          packet_nr++;
        }

        // Critical section
        {
          std::scoped_lock file_lock(chuncks_mutex);
          pcap_data_chunks_.push_back(buffered_packets);
        }

        double processed_percentage =
            100 *
            static_cast<double>((double)current_offset_ / (double)file_size_);
        std::cout << "Processed percentage> : " << processed_percentage
                  << std::endl;
        file_handle_.seekg(current_offset_);
      }

      is_started_.store(false, std::memory_order_release);
    });

    return;
  }

  void stop() {
    std::cout << "# stop buffering " << std::endl;
    is_started_.store(false, std::memory_order_release);
  }

  std::optional<BufferedPackets> next_batch() {
    std::scoped_lock lock_(chuncks_mutex);
    if (pcap_data_chunks_.empty()) {
      return std::nullopt;
    }

    const auto next_chunk = pcap_data_chunks_.front();
    pcap_data_chunks_.pop_front();
    return next_chunk;
  }

  bool is_started() const noexcept {
    return is_started_.load(std::memory_order_acquire);
  }

  std::thread &thread() { return producer_thread_; }

private:
  size_t current_offset_{0};
  size_t file_size_{0};

  std::ifstream &file_handle_;

  std::vector<std::byte> local_buffer_{BATCH_SIZE, std::byte(0x00)};

  std::deque<BufferedPackets> pcap_data_chunks_;
  std::mutex chuncks_mutex;

  std::atomic_bool is_started_{false};
  std::thread producer_thread_{};

  static constexpr size_t BATCH_SIZE = 16 * 1024 * 1024;
  static constexpr bool ENABLE_DEBUGGING{false};
};
} // namespace task::processors::mt_buffer