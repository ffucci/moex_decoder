#include <concepts>

#include "processors/pcap_processor.h"

namespace task::processors {

template <std::invocable<std::span<const std::byte>> Handler>
[[nodiscard]] size_t PCAPProcessor::process_batch(
    PacketProcessor<Handler> &processor) {
  using namespace pcap::types;

  size_t offset = sizeof(pcap::types::pcap_hdr_t);
  size_t total_number_packets = 0;
  while (auto next_batch = pcap_buffer_->next_batch()) {
    std::cout << log_prefix_ << " - consuming batch number (" << batch_number_
              << ")" << std::endl;

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

    std::cout << log_prefix_ << " - Batch number (" << batch_number_
              << ") - Number of read packets " << std::dec
              << next_batch->number_packets << std::endl;
    // fetch enough data to process the current frame
    ++batch_number_;
    total_number_packets += next_batch->number_packets;
    std::this_thread::yield();
  }

  return total_number_packets;
}

}  // namespace task::processors