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

  void start_buffering();

  void stop();

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