#include <cstddef>
#include <iostream>
#include <span>

#include "processors/packet_types.h"
#include "processors/utility.h"

namespace task::processors {

class PacketProcessor {
public:
  void process_packet(std::span<const std::byte> packet,
                      std::invocable auto on_upd_packet) {
    std::span<const std::byte> ethernet_frame{packet.data(), ETH_PACKET_SIZE};
    datamodel::EthernetPacket eth_packet(ethernet_frame);
    size_t offset = ETH_PACKET_SIZE;

    auto ip_span = packet.subspan(offset, 20);
    datamodel::IPPacket ip_packet(ip_span);
    if (packet_processed_ % 100000 == 0) {
      std::cout << "@packet_number " << std::dec << packet_processed_
                << std::endl;
      utility::hex_dump(packet.data(), packet.size(), std::cout);
      std::cout << ip_packet.to_string() << std::endl;
    }
    ++packet_processed_;
  }

private:
  size_t packet_processed_{1};
  static constexpr size_t ETH_PACKET_SIZE = 14;
};

} // namespace task::processors