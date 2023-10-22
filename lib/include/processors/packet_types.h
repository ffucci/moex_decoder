#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <span>
#include <sstream>
#include <string>

namespace task::transport_layer {
struct EthernetPacket {
  EthernetPacket(std::span<const std::byte> buffer);

  static constexpr size_t MAC_ADDRESS_SIZE = 6;
  std::array<std::byte, MAC_ADDRESS_SIZE> dest_address{};
  std::array<std::byte, MAC_ADDRESS_SIZE> source_address{};

  std::array<std::byte, 2> packet_type{};
  static constexpr std::array<std::byte, 2> IP_V4_TYPE = {std::byte{0x80},
                                                          std::byte{0x00}};
};

struct IPPacket {
  std::byte version_ihl{0};
  std::array<std::byte, 2> total_length{};
  std::byte time_to_live{};
  std::byte protocol{};

  uint8_t version{};
  uint8_t ihl{};  // internet header length

  explicit IPPacket(std::span<const std::byte> packet);

  [[nodiscard]] std::string to_string() const noexcept;

  size_t header_length() { return ihl * 4; }
};

}  // namespace task::transport_layer