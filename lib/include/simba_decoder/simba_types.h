#pragma once

#include <_types/_uint16_t.h>
#include <cstdint>
#include <sstream>
#include <string>

namespace task::simba::types {

struct MarketDataPacketHeader {
  uint32_t sequence_number{};
  uint16_t message_size{};
  uint16_t message_flags{};
  uint64_t sending_time{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "market_data_packet_header = (sequence number: "
            << sequence_number << ", message_size: " << message_size;
    sstream << ",message_flags: " << message_flags
            << ", sending_time: " << sending_time << ")";
    return sstream.str();
  }
};

struct IncrementalPacketHeader {
  uint64_t transact_time{};
  uint32_t exchange_session_id{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "incremental_packet_header = (transact time: " << transact_time;
    sstream << ", exchange_session_id: " << exchange_session_id << ")";
    return sstream.str();
  }
};

struct SBEHeader {
  uint16_t block_length{};
  uint16_t template_id{};
  uint16_t schema_id{};
  uint16_t version{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "sbe_header = (block length: " << block_length
            << ", template_id: " << template_id;
    sstream << ", schema_id: " << schema_id << ", version: " << version << ")";
    return sstream.str();
  }
};

} // namespace task::simba::types