#pragma once

#include <cstdint>
#include <sstream>
#include <string>

namespace task::simba::types {

#pragma pack(push, 1)
struct Decimal5 {
  int64_t mantissa{0};
  int8_t exponent{5};
};
#pragma pack(pop)
static_assert(sizeof(Decimal5) == 9);

template <typename Object>
concept Handler = std::invocable<Object>;

enum class MDEntryType : char { Bid, Ask };

enum class MDUpdateAction : uint8_t { New, Delete, Update };

#pragma pack(push, 1)
struct OrderUpdate {
  int64_t md_entry_id{0};
  int64_t order_price{};
  int64_t order_volume{0};
  uint64_t md_flags_set{0};
  uint64_t md_flags_set2{0};
  int32_t security_id{0};
  uint32_t rpt_seq{0};
  MDUpdateAction action{};
  MDEntryType side{};

  [[nodiscard]] std::string to_string() {
    std::stringstream sstream;
    sstream << "order_id:" << md_entry_id
            << " , order_volume: " << order_volume;
    sstream << "order_price:" << order_price << " - " << 5;
    sstream << ", side :" << static_cast<char>(side) << ")";
    return sstream.str();
  }
};
#pragma pack(pop)
static_assert(sizeof(OrderUpdate) == 50);

#pragma pack(push, 1)
struct OrderExecution {
  int64_t order_id{0};
  int64_t order_price{};
  int64_t remaining_quantity{0};
  int64_t trade_price{0}; // this is decimal with constant -5
  int64_t trade_volume{0};
  int64_t trader_id{0};
  uint64_t md_flags_set{0};
  uint64_t md_flags_set2{0};
  int32_t security_id{0};
  uint32_t rpt_seq{0};
  MDUpdateAction action{};
  MDEntryType side{};

  [[nodiscard]] std::string to_string() {
    std::stringstream sstream;
    sstream << "order_id:" << order_id << " , order_volume: " << order_price;
    sstream << ", remaining_quantity:" << remaining_quantity;
    sstream << ", trade_price: "
            << static_cast<double>((double)trade_price / (double)5);
    sstream << ", security id: " << security_id;
    sstream << ", side :" << static_cast<char>(side) << ")";
    return sstream.str();
  }
};
#pragma pack(pop)
static_assert(sizeof(OrderExecution) == 74);

// <group name="NoMDEntries" id="268" dimensionType="groupSize"
// description="Number of entries
//      in Market Data message">
#pragma pack(push, 1)
struct GroupSize {
  uint16_t block_size{0};
  uint8_t num_in_group{0};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "repeating group: ( block_size: " << block_size;
    sstream << ", num_in_group: " << static_cast<int>(num_in_group);
    sstream << ")";
    return sstream.str();
  }
};
#pragma pack(pop)
static_assert(sizeof(GroupSize) == 3);

#pragma pack(push, 1)
struct OrderBookSnapshotHeader {
  int32_t security_id{0};
  uint32_t last_msg_seq_num_processed{0};
  uint32_t rpt_seq{0};
  uint32_t exchange_trading_session_id{0};
  GroupSize group_size{};
  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "order_book_snapshot_header : security_id: " << security_id;
    sstream << ", last_msg_seq_num_processed: " << last_msg_seq_num_processed;
    sstream << ", rpt_seq: " << rpt_seq;
    sstream << ", exchange_trading_session_id: " << exchange_trading_session_id;
    sstream << ", " << group_size.to_string();
    return sstream.str();
  }
};
#pragma pack(pop)
static_assert(sizeof(OrderBookSnapshotHeader) == 19);

// <field name="MDEntryID" id="278" type="Int64NULL" description="Order ID"/>
// <field name="TransactTime" id="60" type="uInt64" description="Start of event
// processing time
//          in number of nanoseconds since Unix epoch, UTC timezone"/>
//  <field name="MDEntryPx" id="270" type="Decimal5NULL" description="Order
//  price"/>
//  <field name="MDEntrySize" id="271" type="Int64NULL" description="Market Data
//  entry size"/> <field name="TradeID" id="1003" type="Int64NULL"
//  description="Trade ID"/> <field name="MDFlags" id="20017" type="MDFlagsSet"
//  description="The field is a bit mask"/> <field name="MDFlags2" id="20050"
//  type="MDFlags2Set" description="The field 2 is a bit mask"/> <field
//  name="MDEntryType" id="269" type="MDEntryType" description="Market Data
//  entry type"/>
#pragma pack(push, 1)
struct OrderBookEntry {
  int64_t order_id{0};
  uint64_t transact_time{0};
  int64_t order_price{0};
  int64_t order_volume{0};
  int64_t trade_id{0};
  uint64_t md_flags_set{0};
  uint64_t md_flags_set2{0};
  MDEntryType side{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "order_book_entry = (order id: " << order_id
            << ", transact_time: " << transact_time;
    sstream << ",order_price: " << order_price
            << ", order_volume: " << order_volume;
    sstream << ", trade_id" << trade_id << ")";
    return sstream.str();
  }
};
#pragma pack(pop)
static_assert(sizeof(OrderBookEntry) == 57);

enum class Messages : uint8_t {
  MessageUnknown = 0,
  OrderUpdateType = 15,
  OrderExecutionType = 16,
  OrderBookSnapshotType = 17
};

#pragma pack(push, 1)
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
#pragma pack(pop)

static_assert(sizeof(MarketDataPacketHeader) == 16);

#pragma pack(push, 1)
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
#pragma pack(pop)
static_assert(sizeof(IncrementalPacketHeader) == 12);

#pragma pack(push, 1)
struct SBEHeader {
  uint16_t block_length{};
  uint16_t template_id{};
  uint16_t schema_id{};
  uint16_t version{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "sbe_header = (block length: " << std::dec << block_length
            << ", template_id: " << std::dec << template_id;
    sstream << ", schema_id: " << std::dec << schema_id
            << ", version: " << std::dec << version << ")";
    return sstream.str();
  }
};
#pragma pack(pop)
static_assert(sizeof(SBEHeader) == 8);

} // namespace task::simba::types