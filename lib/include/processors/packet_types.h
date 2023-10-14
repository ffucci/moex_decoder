#pragma once

#include <_types/_uint16_t.h>
#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <span>
#include <sstream>
#include <string>

namespace task::datamodel {
struct EthernetPacket {
  std::array<std::byte, 6> dest_address{};
  std::array<std::byte, 6> source_address{};
  std::array<std::byte, 2> packet_type{};
  static constexpr std::array<std::byte, 2> IP_V4_TYPE = {std::byte{0x80},
                                                          std::byte{0x00}};

  EthernetPacket(std::span<const std::byte> buffer) {
    std::memcpy(dest_address.data(), buffer.data(), 6);
    std::memcpy(source_address.data(), buffer.data() + 6, 6);
    std::memcpy(packet_type.data(), packet_type.data() + 12, 2);
  }
};

struct IPPacket {
  std::byte version_ihl{0};
  std::array<std::byte, 2> total_length{};

  explicit IPPacket(std::span<const std::byte> packet) {
    size_t offset = 0;
    uint32_t first_row{0};
    std::memcpy(&version_ihl, packet.data(), sizeof(version_ihl));
    offset += sizeof(version_ihl);
    offset += 1;
    std::memcpy(total_length.data(), packet.data() + offset,
                sizeof(total_length));
  }

  std::string to_string() {
    std::stringstream stream;
    auto version = (version_ihl >> 4);
    auto ihl = version_ihl & std::byte{0xF};
    uint16_t converted_total_length =
        (static_cast<uint16_t>(total_length[1]) +
         (static_cast<uint16_t>(total_length[0]) << 8));
    stream << "version: " << (int)version << std::endl
           << "ihl: " << (int)ihl << std::endl
           << "total_length: " << converted_total_length << std::endl;
    return stream.str();
  }
};

} // namespace task::datamodel