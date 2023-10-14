#pragma once

#include <array>
#include <memory>
#include <span>

namespace task::datamodel
{
struct EthernetPacket {
  std::array<std::byte, 6> dest_address{};
  std::array<std::byte, 6> source_address{};
  std::array<std::byte, 2> packet_type{};

  EthernetPacket(std::span<const std::byte> buffer) {
    std::memcpy(dest_address.data(), buffer.data(), 6);
    std::memcpy(source_address.data(), buffer.data() + 6, 6);
    std::memcpy(packet_type.data(), packet_type.data() + 12, 2);
  }
};

}