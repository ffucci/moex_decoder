add_library(task
    cli.cpp
    packet_processor.cpp
    packet_types.cpp
    pcap_processor.cpp
    pcap_buffer.cpp
    simba_decoder.cpp
    utility.cpp)
add_library(task::processors ALIAS task)

target_include_directories(task PUBLIC include)
