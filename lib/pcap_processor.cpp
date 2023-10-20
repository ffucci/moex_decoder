#include "processors/pcap_processor.h"

namespace task::processors {

PCAPProcessor::PCAPProcessor(std::string path,
                             const simba::decoder::MessageHandlers &handlers)
    : decoder(handlers) {
  std::filesystem::path reference{std::move(path)};

  pcap_file_ =
      std::ifstream(reference.string(), std::ios::binary | std::ios::ate);

  if (!pcap_file_)
    throw std::runtime_error(reference.string() + ": " + std::strerror(errno));

  auto end = pcap_file_.tellg();
  std::cout << "FILE NAME > " << reference.string() << std::endl;
  std::cout << "FILE SIZE > " << end << " bytes " << std::endl;

  pcap_file_.seekg(0, std::ios::beg);

  file_size_ = std::size_t(end - pcap_file_.tellg());
  if (file_size_ == 0)
    throw std::runtime_error(ErrorMessage::ERROR_MSG_ZERO_SIZE.data());

  process_header();
  pcap_file_.seekg(HEADER_SIZE);

  // start to produce data
  pcap_buffer_ = std::make_unique<mt_buffer::PCAPBuffer>(pcap_file_, file_size_,
                                                         HEADER_SIZE);

  pcap_buffer_->start_buffering();

  consumer_thread_ = std::thread([this]() {
    std::cout << "Starting consumer thread: " << std::this_thread::get_id()
              << std::endl;

    size_t total_number_packets = 0;

    // decoder handler, calls the decode message when seeing an UDP packet
    std::invocable<std::span<const std::byte>> auto handler =
        [this](std::span<const std::byte> udp_payload) {
          decoder.decode_message(udp_payload);
        };

    PacketProcessor processor(handler);
    while (pcap_buffer_->is_started()) {
      total_number_packets += process_batch(processor);
      std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
    print_end_of_file_info(total_number_packets);
  });

  pcap_buffer_->thread().join();
  consumer_thread_.join();
}

void PCAPProcessor::process_header() {
  std::vector<std::byte> global_header(HEADER_SIZE);

  if (!pcap_file_.read((char *)global_header.data(), HEADER_SIZE))
    throw std::runtime_error("Cannot read the PCAP file header, make sure it "
                             "is a valid PCAP file.");

  pcap::types::pcap_hdr_t header;
  std::memcpy(&header, global_header.data(), global_header.size());

  std::cout << "########## PCAP HEADER #############" << std::endl;
  std::cout << "magic number: " << std::hex << header.magic_number << std::endl;
  std::cout << "version major: " << std::hex << header.version_major
            << std::endl;
  std::cout << "version minor: " << std::hex << header.version_minor
            << std::endl;
  std::cout << "####################################" << std::endl;
}

void PCAPProcessor::print_end_of_file_info(size_t total_packets_number) {
  std::cout << "##### Finished to process file ### " << std::endl;
  std::cout << "# Total number packets of packets processed: "
            << total_packets_number << std::endl;
}

PCAPProcessor::~PCAPProcessor() { pcap_buffer_->stop(); }

} // namespace task::processors