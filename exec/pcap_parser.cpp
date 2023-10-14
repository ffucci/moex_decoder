#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <vector>

#include "dimcli/cli.h"
#include "processors/pcap_processor.h"

int main(int argc, char *argv[]) {
  Dim::Cli cli;

  cli.helpNoArgs();

  auto &pcap_file_path =
      cli.opt<std::string>("file").desc("PCAP file to analyze");

  cli.action([&pcap_file_path](Dim::Cli &) {
    task::processors::PCAPProcessor pcap_processor(
        std::string(pcap_file_path->c_str()));
    return true;
  });

  cli.exec(static_cast<size_t>(argc), argv);
  return cli.exitCode();
}
