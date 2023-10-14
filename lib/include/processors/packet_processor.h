#include <iostream>
#include <span>

#include "processors/packet_types.h"
#include "processors/utility.h"

namespace task::processors {
class PacketProcessor {

public:
  void process_packet(std::span<const std::byte> packet) {
    std::span<const std::byte> ethernet_frame{packet.data(), 14};
    datamodel::EthernetPacket eth_packet(ethernet_frame);
    // task::utility::hex_dump(eth_packet.dest_address.data(), 6, std::cout);
  }
};

} // namespace task::processors