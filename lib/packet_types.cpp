
#include "processors/packet_types.h"
namespace task::transport_layer {

EthernetPacket::EthernetPacket(std::span<const std::byte> buffer) {
  std::memcpy(dest_address.data(), buffer.data(), 6);
  std::memcpy(source_address.data(), buffer.data() + 6, 6);
  std::memcpy(packet_type.data(), packet_type.data() + 12, 2);
}

IPPacket::IPPacket(std::span<const std::byte> packet) {
  size_t offset = 0;
  uint32_t first_row{0};
  std::memcpy(&version_ihl, packet.data(), sizeof(version_ihl));
  offset += sizeof(version_ihl);
  offset += 1;
  std::memcpy(total_length.data(), packet.data() + offset, total_length.size());
  version = static_cast<uint8_t>(version_ihl >> 4);
  ihl = static_cast<uint8_t>(version_ihl & std::byte{0xF});

  offset += total_length.size() + 4;
  std::memcpy(&time_to_live, packet.data() + offset, 1);

  offset += 1;
  std::memcpy(&protocol, packet.data() + offset, 1);
}

[[nodiscard]] std::string IPPacket::to_string() const noexcept {
  std::stringstream stream;
  uint16_t converted_total_length =
      (static_cast<uint16_t>(total_length[1]) +
       (static_cast<uint16_t>(total_length[0]) << 8));
  stream << "version: " << (int)version << std::endl
         << "ihl: " << (int)(ihl * 4) << std::endl
         << "total_length: " << converted_total_length << std::endl
         << "protocol: " << (int)protocol;
  return stream.str();
}
}  // namespace task::transport_layer