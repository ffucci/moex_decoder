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

struct ErrorMessage {
  static constexpr std::string_view ERROR_MSG_ZERO_SIZE =
      "Cannot analyze a file of size zero";
};

class PCAPProcessor {
public:
  PCAPProcessor(std::string path,
                const simba::decoder::MessageHandlers &handlers);

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

  ~PCAPProcessor();

private:
  void process_header();
  void print_end_of_file_info(size_t total_packets_number);

  size_t file_size_{0};
  std::ifstream pcap_file_;
  std::unique_ptr<mt_buffer::PCAPBuffer> pcap_buffer_{};
  std::thread consumer_thread_{};

  simba::decoder::SIMBADecoder decoder;
  static constexpr size_t HEADER_SIZE = sizeof(pcap::types::pcap_hdr_t);
  static constexpr bool ENABLE_DEBUGGING{true};
};
} // namespace task::processors
