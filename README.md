# Overview
The tool has been tested on little endian architecture Intel i5 architecture, on file produced with little endian formats like the ones provided by the MOEX exchange FTP. 

# How to run the tool
The parser expects a PCAP file that is obtained from the MOEX
./pcap_parser --file path/2023-10-10.1359-1406.pcap as an example

The tool supports the following options:
1. *--file:* the input file in PCAP format, it supports only little endian UDP packets encoded using the SIMBA SPECTRA procotol
2. *--out-orders-csv:* the list of OrderExecution and OrderUpdates in the PCAP file. **This input parameter is optional.**
3. *--out-book:* prints the book as reported by the OrderBookSnapshot message. **This input parameter is optional.**

# Tool Architecture
The application is decomposed in a producer thread that chunks and prepare the packets in vector of bytes format that are sent to a consumer thread that process and decodes each single packet.

## Producer/Consumer
The application uses 2 threads, 1 producer and 1 consumer that are synchronized on a atomic variable. The producer (PCAPBuffer) is responsible to buffer the PCAP file. The producer chunks the file in pieces of 16MB each (this value is a constant that can be modified) and decodes the PCAP structure as described in the documentation at this link https://www.ietf.org/archive/id/draft-gharris-opsawg-pcap-01.html.

The *pcap_types.h* header file which defines the PCAP types. The types are the header and the record header that can be used to reconstruct the structure of the packet stream. The PCAP Parser decodes the file in the following structure [GLOBAL_HEADER, PACKET_HEADER1, PACKET_DATA_PAYLOAD1, PACKET_HEADER2, PACKET_DATA_PAYLOAD2, ... , PACKET_HEADERN, PACKET_DATA_PAYLOADN]

## Decoder

The producer analyzes the PCAP packet stream and tries to fit as many packets as possible inside the single chuck of 16MB.
The consumer defined in the *pcap_parser.cpp* source file, processes these packets in form of batches. The consumer thread uses the class *PacketProcessor* to process the upcoming batches.

The packet processing processes and labels the packets contained in the incoming batch in 2 stages:

1. In the first stage the tool decodes the IP packet and checks if it is an UDP packet. In the case the packet is not UDP skips for the processing.
2. If the packet is UDP it tries to decode it according the SIMBA Spectre format. The data types are defined in the *simba_types.h*
headers. The algorithm to decode the SIMBA spectra messages is defined in the *simba_decoder.h* file.


# Produced Output

## Order CSV Example
> ORDER_UPDATE, 2024116201390610024, 13.569, 1, 2101249, 0, 2634189, 20, DELETE, SELL<br>
ORDER_UPDATE, 2024116201390623365, 13.573, 1, 2101249, 0, 2634189, 21, DELETE, SELL<br>
ORDER_UPDATE, 1892948862243751716, 98501, 1, 2101249, 0, 2448082, 292, DELETE, BUY<br>
ORDER_UPDATE, 2024116201390623851, 13.576, 1, 2101249, 0, 2634189, 22, DELETE, SELL<br>
ORDER_UPDATE, 2024116201390609857, 13.574, 1, 2101249, 0, 2634189, 23, DELETE, SELL<br>
ORDER_UPDATE, 2024116201390609858, 13.597, 1, 2101249, 0, 2634189, 24, DELETE, SELL<br>
ORDER_UPDATE, 1892948862243614099, 99198, 1, 2101249, 0, 2448082, 293, DELETE, SELL<br>
ORDER_UPDATE, 1892948862243505853, 100470, 1, 2101249, 0, 2704560, 13, DELETE, SELL<br>
ORDER_UPDATE, 1896045086987432342, 17280, 1, 2101249, 0, 3032018, 19, DELETE, SELL<br>
ORDER_UPDATE, 2024116201390609859, 13.61, 1, 2101249, 0, 2634189, 25, DELETE, SELL<br>
ORDER_UPDATE, 1892948862243519043, 99302, 1, 2101249, 0, 2448082, 294, DELETE, SELL<br>
ORDER_UPDATE, 2024116201390609862, 13.62, 1, 2101249, 0, 2634189, 26, DELETE, SELL<br>
ORDER_UPDATE, 2023553251437207650, 98.42, 1, 2101249, 0, 2634183, 47, DELETE, BUY<br>
...<br>
ORDER_UPDATE, 1892948862244260957, 500, 10, 2101249, 0, 4228358, 7, DELETE, SELL<br>
ORDER_UPDATE, 1951777132375979194, 22.62, 1, 2101249, 0, 3036270, 74, DELETE, SELL<br>
ORDER_UPDATE, 1936577483633591005, 1868.2, 5, 2101249, 0, 3036264, 78, DELETE, BUY<br>
ORDER_UPDATE, 1892948862244474279, 99150, 400, 2, 0, 2448082, 558, NEW, BUY<br>
ORDER_EXECUTION, 1892948862244474279, 99150, 399, 98828, 1, 2199023255554, 0, 2448082, 559, BUY<br>
ORDER_EXECUTION, 1892948862244474249, 98828, 0, 98828, 1, 4398046511105, 0, 2448082, 560, SELL<br>
ORDER_EXECUTION, 1892948862244474279, 99150, 394, 98830, 5, 2199023255554, 0, 2448082, 561, BUY<br>
ORDER_EXECUTION, 1892948862244473438, 98830, 0, 98830, 5, 4398046511105, 0, 2448082, 562, SELL<br>
ORDER_EXECUTION, 1892948862244474279, 99150, 391, 98837, 3, 2199023255554, 0, 2448082, 563, BUY<br>
ORDER_EXECUTION, 1892948862244471528, 98837, 0, 98837, 3, 4398046511105, 0, 2448082, 564, SELL<br>
ORDER_EXECUTION, 1892948862244474279, 99150, 386, 98838, 5, 2199023255554, 0, 2448082, 565, BUY<br>

## Book Snapshot Example
```
order_book_snapshot_header : security_id: 3036264, last_msg_seq_num_processed: 4089, rpt_seq: 24, exchange_trading_session_id: 6902, repeating group: 
( block_size: 57, num_in_group: 23)
----------------------------------------------
| Volume  |       Price      |    Volume     |
----------------------------------------------
|              1931.00000           1       |
|              1926.80000           1       |
|              1922.60000           1       |
|              1901.10000           6       |
|              1897.00000          40       |
|  1           1875.70000                   |
|  2           1875.40000                   |
|  1           1874.00000                   |
|  1           1873.90000                   |
|  1           1873.50000                   |
| 10           1872.60000                   |
|  1           1871.10000                   |
|  1           1871.00000                   |
|  2           1865.10000                   |
|  1           1857.10000                   |
|  1           1854.20000                   |
|  1           1852.80000                   |
|  1           1852.70000                   |
|  1           1852.60000                   |
|  1           1845.70000                   |
|  1           1845.00000                   |
|  1           1843.50000                   |
----------------------------------------------

order_book_snapshot_header : security_id: 3036264, last_msg_seq_num_processed: 4089, rpt_seq: 24, exchange_trading_session_id: 6902, repeating group: 
( block_size: 57, num_in_group: 23)
----------------------------------------------
| Volume  |       Price      |    Volume     |
----------------------------------------------
|              1932.00000          25       |
|              1899.00000         100       |
|              1898.30000          50       |
|              1897.80000           1       |
|              1895.00000           2       |
|              1892.50000           1       |
|  1           1875.10000                   |
|  1           1875.00000                   |
|  1           1874.20000                   |
|  1           1871.80000                   |
|  1           1871.30000                   |
|677           1870.10000                   |
| 15           1867.30000                   |
|  3           1865.00000                   |
|  2           1864.00000                   |
|  1           1860.80000                   |
|  1           1857.50000                   |
|  1           1855.00000                   |
| 25           1826.00000                   |
----------------------------------------------
```


## Log Example

> FILE NAME > /Users/francesco/Downloads/2023-10-10.0845-0905.pcap<br>
> FILE SIZE > 1996408844 bytes 
>
> ########## PCAP HEADER #############<br>
magic number: a1b23c4d<br>
version major: 2<br>
version minor: 4<br>
####################################<br>
[PCAP_BUFFER] Start buffering <br>
Starting consumer thread: 0x70000e7b9000<br>
[PCAP_BUFFER] Percentage of the whole file processed (0.84037%)<br>
[PCAP_PROCESSOR] - consuming batch number (1)<br>
[PCAP_BUFFER] Percentage of the whole file processed (1.6807%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (2.52106%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (3.36143%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (4.20177%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (5.04214%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (5.88249%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (6.7228%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (7.56314%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (8.40348%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (9.2438%)<br>
[PCAP_PROCESSOR] - Batch number (1) - Number of read packets 36046<br>
[PCAP_PROCESSOR] - consuming batch number (2)<br>
[PCAP_BUFFER] Percentage of the whole file processed (10.0842%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (10.9245%)<br>
[PCAP_BUFFER] Percentage of the whole file processed (11.7649%)<br>