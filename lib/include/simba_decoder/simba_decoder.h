#pragma once

#include <iostream>
#include <span>

#include "simba_decoder/simba_types.h"

namespace task::simba::decoder {
struct SIMBADecoder {
  SIMBADecoder(std::span<const std::byte> udp_payload) {
    auto span_market_header = udp_payload.subspan(0, sizeof(packet_header));
    std::memcpy(&packet_header, span_market_header.data(),
                span_market_header.size());
    current_offset_ += span_market_header.size();
    std::cout << packet_header.to_string() << std::endl;
    if (packet_header.message_flags & 0x8) {
      types::IncrementalPacketHeader inc_header;
      auto span_incremental_header = udp_payload.subspan(current_offset_, 12);
      current_offset_ += 12;
      std::cout << "offset_ :" << std::dec << current_offset_ << std::endl;
      std::memcpy(&inc_header, span_incremental_header.data(),
                  span_incremental_header.size());
      incremental_header = inc_header;
      std::cout << incremental_header->to_string() << std::endl;
    }

    // Reading the SBE Header
    auto sbe_header_span =
        udp_payload.subspan(current_offset_, sizeof(sbe_header));
    current_offset_ += sbe_header_span.size();
    std::memcpy(&sbe_header, sbe_header_span.data(), sbe_header_span.size());
    std::cout << sbe_header.to_string() << std::endl;
  }

  simba::types::MarketDataPacketHeader packet_header;
  std::optional<simba::types::IncrementalPacketHeader> incremental_header;
  types::SBEHeader sbe_header{};

  size_t current_offset_{0};
};
} // namespace task::simba::decoder