#include <_stdio.h>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <thread>
#include <vector>

#include "processors/packet_processor.h"
#include "processors/pcap_buffer.h"
#include "processors/pcap_types.h"
#include "processors/utility.h"
#include "simba_decoder/simba_decoder.h"
#include "simba_decoder/simba_types.h"

namespace task::processors {
class PCAPProcessor {
public:
  PCAPProcessor(std::string path) {
    std::filesystem::path reference{std::move(path)};
    pcap_file_ =
        std::ifstream(reference.string(), std::ios::binary | std::ios::ate);

    if (!pcap_file_)
      throw std::runtime_error(reference.string() + ": " +
                               std::strerror(errno));

    auto end = pcap_file_.tellg();
    std::cout << "FILE NAME > " << reference.string() << std::endl;
    std::cout << "FILE SIZE > " << end << " bytes " << std::endl;

    pcap_file_.seekg(0, std::ios::beg);

    file_size_ = std::size_t(end - pcap_file_.tellg());
    if (file_size_ == 0)
      throw std::runtime_error("Cannot analyze a file of zero dimension");

    process_header();
    pcap_file_.seekg(HEADER_SIZE);

    // start to produce data
    pcap_buffer_ = std::make_unique<mt_buffer::PCAPBuffer>(
        pcap_file_, file_size_, HEADER_SIZE);

    pcap_buffer_->start_buffering();

    consumer_thread_ = std::thread([this]() {
      std::cout << "Starting consumer thread: " << std::this_thread::get_id()
                << std::endl;

      size_t total_number_packets = 0;
      std::invocable<std::span<const std::byte>> auto handler =
          [this](std::span<const std::byte> udp_payload) {
            decoder.decode_message(udp_payload);
          };
      PacketProcessor processor(handler);
      while (pcap_buffer_->is_started()) {
        total_number_packets += process_batch(processor);
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
      }
      std::cout << "##### Finished to process file ### " << std::endl;
      std::cout << "Total number packets: " << total_number_packets
                << std::endl;
    });

    pcap_buffer_->thread().join();
    consumer_thread_.join();
  }

  template <std::invocable<std::span<const std::byte>> Handler>
  [[nodiscard]] size_t process_batch(PacketProcessor<Handler> &processor) {
    using namespace pcap::types;

    size_t offset = sizeof(pcap::types::pcap_hdr_t);
    size_t batch_number = 1;
    size_t total_number_packets = 0;
    while (auto next_batch = pcap_buffer_->next_batch()) {
      std::cout << "### BATCH NUMBER " << batch_number << std::endl;

      size_t packet_nr_in_batch{0};
      for (auto &packet : next_batch->packets) {
        processor.process_packet(packet);

        if constexpr (ENABLE_DEBUGGING) {
          if (packet_nr_in_batch == 0) {
            std::cout << "@ start packet_number: "
                      << next_batch->start_packet_number << std::endl;
            utility::hex_dump(packet.data(), packet.size(), std::cout);
          }
        }

        ++packet_nr_in_batch;
      }

      std::cout << "Number of read packets " << std::dec
                << next_batch->number_packets << std::endl;
      // fetch enough data to process the current frame
      ++batch_number;
      total_number_packets += next_batch->number_packets;
      std::this_thread::yield();
    }

    return total_number_packets;
  }

private:
  void process_header() {
    std::vector<std::byte> global_header(HEADER_SIZE);

    if (!pcap_file_.read((char *)global_header.data(), HEADER_SIZE))
      throw std::runtime_error("Cannot PCAP file read header.");

    pcap::types::pcap_hdr_t header;
    std::memcpy(&header, global_header.data(), global_header.size());

    std::cout << "########## PCAP HEADER #############" << std::endl;
    std::cout << "magic number: " << std::hex << header.magic_number
              << std::endl;
    std::cout << "version major: " << std::hex << header.version_major
              << std::endl;
    std::cout << "version minor: " << std::hex << header.version_minor
              << std::endl;
    std::cout << "####################################" << std::endl;
  }

  size_t file_size_{0};

  std::ifstream pcap_file_;

  std::unique_ptr<mt_buffer::PCAPBuffer> pcap_buffer_{};

  std::thread consumer_thread_{};

  simba::decoder::SIMBADecoder decoder;
  static constexpr size_t HEADER_SIZE = sizeof(pcap::types::pcap_hdr_t);
  static constexpr bool ENABLE_DEBUGGING{true};
};
} // namespace task::processors
