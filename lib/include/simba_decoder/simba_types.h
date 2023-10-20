#pragma once

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace task::simba::types {

static constexpr uint64_t NULL_VALUE = 9223372036854775807;
static constexpr double SIMBA_MOEX_EXPONENT = 1e5;

constexpr double to_normalized_price(int64_t price) {
  return static_cast<double>((static_cast<double>(price)) /
                             SIMBA_MOEX_EXPONENT);
}

template <typename Object>
concept Handler = std::invocable<Object>;

#pragma pack(push, 1)
struct Decimal5 {
  int64_t mantissa{0};
  int8_t exponent{-5};
};
#pragma pack(pop)
static_assert(sizeof(Decimal5) == 9);

enum class MDEntryType : char { Bid = '0', Offer = '1', EmptyBook = 'J' };

constexpr MDEntryType from_char(char entry) {
  switch (entry) {
  case '0': {
    return MDEntryType::Bid;
  }
  case '1': {
    return MDEntryType::Offer;
  }
  case 'J': {
    return MDEntryType::EmptyBook;
  }
  default:
    throw std::runtime_error("Unexpected entry");
  }
}

constexpr std::string_view entry_side_to_string(MDEntryType entry) {
  switch (entry) {
  case MDEntryType::Bid: {
    return "BUY";
  }
  case MDEntryType::Offer: {
    return "SELL";
  }
  case MDEntryType::EmptyBook: {
    return "EMPTY_BOOK";
  }
  default:
    throw std::runtime_error("Unexpected entry");
  }
}

enum class MDUpdateAction : uint8_t { New = 0, Update, Delete };
constexpr std::string_view update_action_to_string(MDUpdateAction entry) {
  switch (entry) {
  case MDUpdateAction::New: {
    return "NEW";
  }
  case MDUpdateAction::Update: {
    return "UPDATE";
  }
  case MDUpdateAction::Delete: {
    return "DELETE";
  }
  default:
    throw std::runtime_error("Unexpected MDUpdateAction entry");
  }
}

#pragma pack(push, 1)
struct OrderUpdate {
  int64_t order_id{0};
  int64_t order_price{};
  int64_t order_volume{0};
  uint64_t md_flags_set{0};
  uint64_t md_flags_set2{0};
  int32_t security_id{0};
  uint32_t rpt_seq{0};
  MDUpdateAction action{};
  MDEntryType side{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "order_id:" << order_id << " , order_volume: " << order_volume;
    sstream << "order_price:" << order_price << " - " << 5;
    sstream << ", side :" << static_cast<char>(side) << ")";
    return sstream.str();
  }

  [[nodiscard]] std::string to_csv_string() const noexcept {
    std::stringstream sstream;
    sstream << order_id;
    auto normalized_order_price =
        (order_price != NULL_VALUE ? to_normalized_price(order_price)
                                   : std::nan("null value"));

    sstream << ", " << normalized_order_price;
    sstream << ", " << order_volume;

    sstream << ", " << md_flags_set;
    sstream << ", " << md_flags_set2;

    sstream << ", " << security_id;
    sstream << ", " << rpt_seq;
    sstream << ", " << update_action_to_string(action);
    sstream << ", " << entry_side_to_string(side);
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
  int64_t trade_price{
      0}; // this is decimal with constant -5 (this might be null)
  int64_t trade_volume{0};
  int64_t trader_id{0};
  uint64_t md_flags_set{0};
  uint64_t md_flags_set2{0};
  int32_t security_id{0};
  uint32_t rpt_seq{0};
  MDUpdateAction action{};
  MDEntryType side{};

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    sstream << "order_id:" << order_id
            << " , order_price: " << to_normalized_price(order_price);
    sstream << ", remaining_quantity:" << remaining_quantity;
    sstream << ", trade_price: "
            << static_cast<double>((double)trade_price * SIMBA_MOEX_EXPONENT);
    sstream << ", security id: " << security_id;
    sstream << ", side :" << static_cast<char>(side) << ")";
    return sstream.str();
  }

  [[nodiscard]] std::string to_csv_string() const noexcept {
    std::stringstream sstream;
    sstream << order_id;
    auto normalized_order_price =
        (order_price != NULL_VALUE ? to_normalized_price(order_price)
                                   : std::nan("null value"));
    sstream << ", " << normalized_order_price;
    sstream << ", " << remaining_quantity;

    sstream << ", " << to_normalized_price(trade_price);
    sstream << ", " << trade_volume;

    sstream << ", " << md_flags_set;
    sstream << ", " << md_flags_set2;

    sstream << ", " << security_id;
    sstream << ", " << rpt_seq;

    sstream << ", " << entry_side_to_string(side);
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
    sstream << ",order_price: "
            << ((order_price == NULL_VALUE) ? std::string("NULL")
                                            : std::to_string(order_price))
            << ", order_volume: " << order_volume;
    sstream << ", side: " << static_cast<char>(side);
    sstream << ", trade_id: " << trade_id << ")";
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
    sstream << ", message_flags: " << message_flags
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

class OrderBookSnapshot {
public:
  OrderBookSnapshot(const OrderBookSnapshotHeader &snapshot_header)
      : snapshot_header_(snapshot_header) {}

  void insert(OrderBookEntry &&entry) {
    if (entry.order_price == NULL_VALUE) {
      return;
    }

    const auto index = to_index(entry.side);
    if (entry.side == MDEntryType::EmptyBook) {
      return;
    }

    if (entry.side == MDEntryType::Bid) {
      bid_book_.emplace(entry.order_price, entry);
    } else {
      ask_book_.emplace(entry.order_price, entry);
    }
  }

  [[nodiscard]] std::string to_string() const noexcept {
    std::stringstream sstream;
    int32_t price_width = 10, quantity_width = 12;
    sstream << std::resetiosflags(std::ios::left);
    sstream << snapshot_header_.to_string() << '\n';
    sstream << "----------------------------------------------" << '\n';
    sstream << std::setw(3) << "| Volume ";
    sstream << " |       Price      | ";
    sstream << std::setw(3) << " ";
    sstream << "Volume"
            << "     |";
    sstream << "\n";
    sstream << "----------------------------------------------" << '\n';

    auto ask_iterator = ask_book_.rbegin();
    sstream << std::setiosflags(std::ios::right);

    while (ask_iterator != ask_book_.rend()) {
      sstream << std::right << "|";

      sstream << std::setw(4) << " ";

      sstream << std::right << std::fixed << std::setprecision(5)
              << std::setw(price_width) << "  "
              << to_normalized_price(ask_iterator->first);

      sstream << std::setw(quantity_width) << ask_iterator->second.order_volume;
      sstream << std::setw(price_width - 2) << std::right << "|";
      sstream << "\n";
      ++ask_iterator;
    }

    auto bid_iterator = bid_book_.begin();
    while (bid_iterator != bid_book_.end()) {
      sstream << std::right << "|";
      sstream << std::right << std::setw(3)
              << bid_iterator->second.order_volume;
      sstream << std::setw(price_width + 1) << std::right << std::fixed
              << std::setprecision(5) << " "
              << to_normalized_price(bid_iterator->first);
      sstream << std::setw(quantity_width + price_width - 2) << std::right
              << " |";
      sstream << "\n";
      ++bid_iterator;
    }
    sstream << "----------------------------------------------" << '\n';

    return sstream.str();
  }

private:
  constexpr uint8_t to_index(MDEntryType side) {
    switch (side) {
    case MDEntryType::Bid: {
      return 0;
    }
    case MDEntryType::Offer: {
      return 1;
    }
    case MDEntryType::EmptyBook: {
      return 2;
    }
    default: {
      throw std::runtime_error("Cannot convert into index the book side");
    }
    }
  }

  OrderBookSnapshotHeader snapshot_header_{};
  std::map<int64_t, OrderBookEntry, std::greater<>> bid_book_{};
  std::map<int64_t, OrderBookEntry> ask_book_{};
};

} // namespace task::simba::types