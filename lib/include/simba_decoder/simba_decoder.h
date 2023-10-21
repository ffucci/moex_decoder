#pragma once

#include <cassert>
#include <cstdio>
#include <iostream>
#include <span>
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

class SIMBADecoder {
 public:
  SIMBADecoder() = delete;

  explicit SIMBADecoder(const MessageHandlers &message_handler)
      : message_handlers_(message_handler) {}

  void decode_message(std::span<const std::byte> udp_payload);

  [[nodiscard]] const types::MarketDataPacketHeader &market_header()
      const noexcept {
    return market_update_header_;
  }

  [[nodiscard]] const types::SBEHeader &sbe_header() const noexcept {
    return sbe_header_;
  }

 private:
  [[nodiscard]] size_t handle_order_update(
      const types::SBEHeader &sbe_header,
      std::span<const std::byte> udp_payload);

  [[nodiscard]] size_t handle_order_execution(
      const types::SBEHeader &sbe_header,
      std::span<const std::byte> udp_payload);

  void handle_order_book_snapshot(std::span<const std::byte> udp_payload);
  types::Messages from_id_to_type(uint16_t template_id) {
    return mapping_[template_id];
  }

  size_t current_offset_{0};

  simba::types::MarketDataPacketHeader market_update_header_{};
  std::optional<simba::types::IncrementalPacketHeader> incremental_header{};
  types::SBEHeader sbe_header_{};

  std::unordered_map<uint64_t, types::Messages> mapping_ = {
      {15, types::Messages::OrderUpdateType},
      {16, types::Messages::OrderExecutionType},
      {17, types::Messages::OrderBookSnapshotType}};
  MessageHandlers message_handlers_{};

  static constexpr bool ENABLE_DEBUGGING{false};
};
}  // namespace task::simba::decoder