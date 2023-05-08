from scapy.all import *

# Set packet parameters
src_ip = "192.168.1.1"
dst_ip = "192.168.1.2"
src_port = 1234
dst_port = 80
payload = "Hello, World!"


# Create packets
while True:
    packet = IP(src=src_ip, dst=dst_ip) / TCP(sport=src_port, dport=dst_port) / payload
    send(packet)