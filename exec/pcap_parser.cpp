#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <vector>

#include "dimcli/cli.h"
#include "processors/pcap_processor.h"
#include "simba_decoder/simba_decoder.h"
#include "simba_decoder/simba_types.h"

int main(int argc, char *argv[]) {
  Dim::Cli cli;

  cli.helpNoArgs();

  auto &pcap_file_path =
      cli.opt<std::string>("file").desc("PCAP file to analyze");
  auto &out_csv_path = cli.opt<std::string>("?out-csv")
                           .desc("Decoded CSV output for incremental stream");
  auto &out_snapshot_log_path =
      cli.opt<std::string>("?out-book")
          .desc("Decoded OrderBook for order book snapshot");

  if (!cli.parse(argc, argv)) {
    return cli.printError(std::cerr);
  }

  std::optional<std::ofstream> decoded_stream_csv{std::nullopt};
  if (out_csv_path) {
    std::filesystem::path decoded_csv_file(*out_csv_path);
    decoded_stream_csv = std::ofstream(decoded_csv_file.string());
  }

  std::optional<std::ofstream> output_book_file_stream{std::nullopt};
  if (out_snapshot_log_path) {
    std::filesystem::path output_book_file(*out_snapshot_log_path);
    output_book_file_stream = std::ofstream(output_book_file.string());
  }

  cli.action([&pcap_file_path, &decoded_stream_csv,
              &output_book_file_stream](Dim::Cli &) {
    task::simba::decoder::MessageHandlers handlers;
    if (decoded_stream_csv) {
      handlers.order_execution_handler =
          [&decoded_stream_csv](
              const task::simba::types::OrderExecution &order_execution) {
            *decoded_stream_csv << "ORDER_EXECUTION, "
                                << order_execution.to_csv_string() << std::endl;
          };
      handlers.order_update_handler =
          [&decoded_stream_csv](
              const task::simba::types::OrderUpdate &order_update) {
            *decoded_stream_csv << "ORDER_UPDATE, "
                                << order_update.to_csv_string() << std::endl;
          };
    }

    if (output_book_file_stream) {
      handlers.order_book_snapshot_handler =
          [&output_book_file_stream](
              const task::simba::types::OrderBookSnapshot &book) {
            *output_book_file_stream << book.to_string() << std::endl;
          };
    }

    task::processors::PCAPProcessor pcap_processor(
        std::string(pcap_file_path->c_str()), handlers);
    return true;
  });

  cli.exec(static_cast<size_t>(argc), argv);
  return cli.exitCode();
}
