include /usr/local/etc/PcapPlusPlus.mk

# All Target
# g++ $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -I/packetCapture/ -c -o packetCapture.o packetCapture.cpp
all:
	g++ $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c packetCapture/packetCapture.cpp -I packetCapture -I ./ -I ./dependencies/include -o packetCapture/packetCapture.o
	g++ $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -I ./dependencies/include -o main.o main.cpp
	g++ $(PCAPPP_LIBS_DIR) -static-libstdc++ -o packet-capture main.o ./packetCapture/packetCapture.o $(PCAPPP_LIBS)

# Clean Target
clean:
	rm main.o
	rm packet-capture