#pragma once

#include <cstddef>
#include <iostream>
#include <span>

#include "processors/packet_types.h"
#include "processors/utility.h"

namespace task::processors {

template <std::invocable<std::span<const std::byte>> UDPPacketHandler>
class PacketProcessor {

public:
  PacketProcessor(const UDPPacketHandler &on_upd_packet)
      : udp_packet_handler_(on_upd_packet) {}

  void process_packet(std::span<const std::byte> packet);

private:
  UDPPacketHandler udp_packet_handler_;
  size_t packet_processed_{1};
  static constexpr size_t ETH_PACKET_SIZE = 14;
  static constexpr size_t UDP_HEADER_SIZE = 8;

  static constexpr std::byte UDP_PROTOCOL = std::byte{0x11};
  static constexpr bool ENABLE_DEBUGGING{false};
};

} // namespace task::processors

#include "processors/packet_processor.hpp"
