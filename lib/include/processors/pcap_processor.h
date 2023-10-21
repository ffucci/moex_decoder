#pragma once

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string_view>
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
  [[nodiscard]] size_t process_batch(PacketProcessor<Handler> &processor);

  ~PCAPProcessor();

private:
  void process_header();
  void print_end_of_file_info(size_t total_packets_number);

  size_t batch_number_{1};
  size_t file_size_{0};
  std::ifstream pcap_file_;
  std::unique_ptr<mt_buffer::PCAPBuffer> pcap_buffer_{};
  std::thread consumer_thread_{};

  simba::decoder::SIMBADecoder decoder;

  static constexpr size_t CONSUMER_BUFFERING_TIME{500};
  static constexpr size_t HEADER_SIZE = sizeof(pcap::types::pcap_hdr_t);
  static constexpr bool ENABLE_DEBUGGING{false};

  static constexpr std::string_view log_prefix_{"[PCAP_PROCESSOR]"};
};
} // namespace task::processors

#include "processors/pcap_processor.hpp"
