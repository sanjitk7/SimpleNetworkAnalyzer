## Tasks for PacketAnalyzer

- .pcap files to .json - Elastic stack Visualize
- Network simulation - simple looped repeated queries to example.com
- pcapplusplus - TCP packet statistics - packet loss (seq number no miss), packet delay (time bw conseq packets), retrasmission

packet loss: prev-sequence-number  - curr-sequence-number 

every sample_time:
    collect sample_size number of packets:
        average_packet_loss
        average_packet_delay 
        average_retransmitted_packets
  
collection of network packet metadata:
    1. Timestamps
    2. Packet Size
    3. Source IP
    4. Dest IP
    5. Port
    6. Seq Number
    7. ACK no
    8. Retransmission count
    9. window size


[{"ts":12, "size":133....},{},...]