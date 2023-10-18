#pragma once

#include <_types/_uint16_t.h>
#include <_types/_uint64_t.h>
#include <iostream>
#include <span>
#include <sys/_types/_size_t.h>
#include <unordered_map>

#include "simba_decoder/simba_types.h"

namespace task::simba::decoder {

struct MessageHandlers {};

struct SIMBADecoder {
  SIMBADecoder() = default;

  types::Messages from_id_to_type(uint16_t template_id) {
    return mapping_[template_id];
  }

  void decode_message(std::span<const std::byte> udp_payload) {
    size_t current_offset_{0};
    auto span_market_header = udp_payload.subspan(0, sizeof(packet_header));
    std::memcpy(&packet_header, span_market_header.data(),
                span_market_header.size());
    current_offset_ += span_market_header.size();
    std::cout << packet_header.to_string() << std::endl;
    if (packet_header.message_flags & 0x8) {
      types::IncrementalPacketHeader inc_header;
      auto span_incremental_header = udp_payload.subspan(current_offset_, 12);
      current_offset_ += 12;
      std::memcpy(&inc_header, span_incremental_header.data(),
                  span_incremental_header.size());
      incremental_header = inc_header;
      std::cout << incremental_header->to_string() << std::endl;
    }

    // Reading the SBE Header
    auto sbe_header_span =
        udp_payload.subspan(current_offset_, sizeof(sbe_header));
    std::memcpy(&sbe_header, sbe_header_span.data(), sbe_header_span.size());
    std::cout << sbe_header.to_string() << std::endl;
    current_offset_ += sizeof(sbe_header);

    auto template_type = from_id_to_type(sbe_header.template_id);

    auto message_span =
        udp_payload.subspan(current_offset_, sbe_header.block_length);
    switch (template_type) {
    case types::Messages::OrderUpdateType: {

      types::OrderUpdate order_update;
      std::memcpy(&order_update, message_span.data(), message_span.size());
      std::cout << "order update (" << std::dec << sizeof(types::OrderUpdate)
                << ") = " << order_update.to_string() << std::endl;
      break;
    }
    case types::Messages::OrderExecutionType: {

      types::OrderExecution order_execution;
      std::memcpy(&order_execution, message_span.data(), message_span.size());
      std::cout << "order execution (" << std::dec
                << sizeof(types::OrderExecution) << ") "
                << order_execution.to_string() << std::endl;
      break;
    }
    case types::Messages::OrderBookSnapshotType: {

      types::OrderBookSnapshotHeader order_book_snapshot_header;
      std::memcpy(&order_book_snapshot_header, message_span.data(),
                  message_span.size() + sizeof(types::GroupSize));
      std::cout << order_book_snapshot_header.to_string() << std::endl;
      current_offset_ += message_span.size();
      const auto num_in_group =
          order_book_snapshot_header.group_size.num_in_group;
      std::vector<types::OrderBookEntry> book_entries(num_in_group);
      size_t entry_nr = 0;
      while (entry_nr < num_in_group) {
        types::OrderBookEntry entry;
        auto span_entry = udp_payload.subspan(
            current_offset_, order_book_snapshot_header.group_size.block_size);
        std::memcpy(&entry, span_entry.data(), span_entry.size());
        std::cout << std::endl;
        ++entry_nr;
      }
      break;
    }
    default:
      break;
    }
  }

  simba::types::MarketDataPacketHeader packet_header;
  std::optional<simba::types::IncrementalPacketHeader> incremental_header;
  types::SBEHeader sbe_header{};

  std::unordered_map<uint64_t, types::Messages> mapping_ = {
      {15, types::Messages::OrderUpdateType},
      {16, types::Messages::OrderExecutionType},
      {17, types::Messages::OrderBookSnapshotType}};
  MessageHandlers handlers{};
};
} // namespace task::simba::decoder