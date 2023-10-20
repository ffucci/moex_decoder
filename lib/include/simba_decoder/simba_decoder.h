#pragma once

#include <cassert>
#include <cstdio>
#include <iostream>
#include <span>
#include <sys/_types/_size_t.h>
#include <type_traits>
#include <unordered_map>

#include "simba_decoder/simba_types.h"

namespace task::simba::decoder {

struct MessageHandlers {
  std::function<void(const types::OrderExecution &)> order_execution_handler;
  std::function<void(const types::OrderUpdate &)> order_update_handler;
  std::function<void(const types::OrderBookSnapshot &)>
      order_book_snapshot_handler;
};

struct SIMBADecoder {
  SIMBADecoder() = delete;

  explicit SIMBADecoder(const MessageHandlers &message_handler)
      : message_handlers_(message_handler) {}

  types::Messages from_id_to_type(uint16_t template_id) {
    return mapping_[template_id];
  }

  void decode_message(std::span<const std::byte> udp_payload) {
    constexpr auto INCREMENTAL_HEADER_SIZE{
        sizeof(types::IncrementalPacketHeader)};

    // Reset decoder offset
    current_offset_ = 0;

    // Read the MarketPacketHeader
    auto span_market_header =
        udp_payload.subspan(0, sizeof(market_update_header_));
    std::memcpy(&market_update_header_, span_market_header.data(),
                span_market_header.size());
    current_offset_ += span_market_header.size();

    if constexpr (ENABLE_DEBUGGING) {
      std::cout << market_update_header_.to_string() << std::endl;
    }

    // Read the IncrementalPacketHeader if available
    if (market_update_header_.message_flags & 0x8) {
      types::IncrementalPacketHeader inc_header;
      auto span_incremental_header =
          udp_payload.subspan(current_offset_, INCREMENTAL_HEADER_SIZE);
      current_offset_ += INCREMENTAL_HEADER_SIZE;
      std::memcpy(&inc_header, span_incremental_header.data(),
                  span_incremental_header.size());
      incremental_header = inc_header;

      if constexpr (ENABLE_DEBUGGING) {
        std::cout << incremental_header->to_string() << std::endl;
      }
    }

    // Reading the SBE Header
    bool to_be_skipped{
        false}; // We skip the messages that are not of our interest
    while (current_offset_ < market_update_header_.message_size &&
           !to_be_skipped) {
      auto sbe_header_span =
          udp_payload.subspan(current_offset_, sizeof(sbe_header));
      std::memcpy(&sbe_header, sbe_header_span.data(), sbe_header_span.size());
      if constexpr (ENABLE_DEBUGGING) {
        std::cout << sbe_header.to_string() << std::endl;
      }
      current_offset_ += sizeof(sbe_header);

      auto template_type = from_id_to_type(sbe_header.template_id);

      switch (template_type) {
      case types::Messages::OrderUpdateType: {
        current_offset_ += handle_order_update(sbe_header, udp_payload);
        break;
      }
      case types::Messages::OrderExecutionType: {
        current_offset_ += handle_order_execution(sbe_header, udp_payload);
        break;
      }
      case types::Messages::OrderBookSnapshotType: {
        handle_order_book_snapshot(udp_payload);
        break;
      }
      default:
        to_be_skipped = true;
        break;
      }
    }
  }

  [[nodiscard]] size_t
  handle_order_update(const types::SBEHeader &sbe_header,
                      std::span<const std::byte> udp_payload) {
    auto message_span =
        udp_payload.subspan(current_offset_, sbe_header.block_length);
    types::OrderUpdate order_update;
    std::memcpy(&order_update, message_span.data(), message_span.size());
    if (message_handlers_.order_update_handler) {
      message_handlers_.order_update_handler(order_update);
    }

    return sizeof(types::OrderUpdate);
  }

  [[nodiscard]] size_t
  handle_order_execution(const types::SBEHeader &sbe_header,
                         std::span<const std::byte> udp_payload) {
    auto message_span =
        udp_payload.subspan(current_offset_, sbe_header.block_length);
    types::OrderExecution order_execution;
    std::memcpy(&order_execution, message_span.data(), message_span.size());
    if (message_handlers_.order_execution_handler) {
      message_handlers_.order_execution_handler(order_execution);
    }
    return sizeof(types::OrderExecution);
  }

  void handle_order_book_snapshot(std::span<const std::byte> udp_payload) {
    auto message_span =
        udp_payload.subspan(current_offset_, sbe_header.block_length);
    types::OrderBookSnapshotHeader order_book_snapshot_header;
    std::memcpy(&order_book_snapshot_header, message_span.data(),
                message_span.size() + sizeof(types::GroupSize));
    current_offset_ += message_span.size() + sizeof(types::GroupSize);
    const auto num_in_group =
        order_book_snapshot_header.group_size.num_in_group;
    std::vector<types::OrderBookEntry> book_entries(num_in_group);
    size_t entry_nr = 0;

    types::OrderBookSnapshot snapshot(order_book_snapshot_header);

    // Decode each book entry
    while (entry_nr < num_in_group) {
      types::OrderBookEntry entry;
      auto span_entry = udp_payload.subspan(
          current_offset_, order_book_snapshot_header.group_size.block_size);
      std::memcpy(&entry, span_entry.data(), span_entry.size());
      current_offset_ += span_entry.size();
      snapshot.insert(std::move(entry));
      ++entry_nr;
    }

    if (message_handlers_.order_book_snapshot_handler) {
      message_handlers_.order_book_snapshot_handler(snapshot);
    }
  }

  size_t current_offset_{0};

  simba::types::MarketDataPacketHeader market_update_header_{};
  std::optional<simba::types::IncrementalPacketHeader> incremental_header{};
  types::SBEHeader sbe_header{};

  std::unordered_map<uint64_t, types::Messages> mapping_ = {
      {15, types::Messages::OrderUpdateType},
      {16, types::Messages::OrderExecutionType},
      {17, types::Messages::OrderBookSnapshotType}};
  MessageHandlers message_handlers_{};

  static constexpr bool ENABLE_DEBUGGING{false};
};
} // namespace task::simba::decoder