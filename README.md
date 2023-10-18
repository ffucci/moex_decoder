tcpdump -qns 0 -x -A -r ../data/ipv4frags.pcap

# How to run the parser
./pcap_parser --file /Users/francesco/Downloads/2023-10-10.1359-1406.pcap > production_2.log