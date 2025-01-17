#include "processors/packet_processor.h"

namespace task::processors {

template <std::invocable<std::span<const std::byte>> UDPPacketHandler>
void PacketProcessor<UDPPacketHandler>::process_packet(
    std::span<const std::byte> packet) {
  std::span<const std::byte> ethernet_frame{packet.data(), ETH_PACKET_SIZE};
  transport_layer::EthernetPacket eth_packet(ethernet_frame);
  size_t offset = ETH_PACKET_SIZE;

  auto ip_span = packet.subspan(offset, 80);
  transport_layer::IPPacket ip_packet(ip_span);

  if constexpr (ENABLE_DEBUGGING) {
    if (packet_processed_ % 100000 == 0) {
      std::cout << "@ packet_number " << std::dec << packet_processed_
                << std::endl;
      utility::hex_dump(packet.data(), packet.size(), std::cout);
      std::cout << ip_packet.to_string() << std::endl;
    }
  }

  // UDP header start
  offset += ip_packet.header_length();

  // SKIP TCP packets because the tool does not support them
  if (ip_packet.protocol != UDP_PROTOCOL) {
    std::cout << "[PACKET_PROCESSOR] - skipping non-UDP packets since the tool "
                 "does not support such packets"
              << std::endl;
    return;
  }

  if constexpr (ENABLE_DEBUGGING) {
    if (packet_processed_ % 100000 == 0) {
      std::cout << "[PACKET_PROCESSOR] - UDP PACKET SIZE: " << std::dec
                << packet.size() - offset << std::endl;
    }
  }
  offset += UDP_HEADER_SIZE;
  auto udp_span = packet.subspan(offset);
  udp_packet_handler_(udp_span);
  ++packet_processed_;
}
}  // namespace task::processors