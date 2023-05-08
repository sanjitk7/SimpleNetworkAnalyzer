#include <iostream>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"

/**
* A callback function for the async capture which is called each time a packet is captured
*/
static void onPacketArrives(pcpp::RawPacket* rawPacket, pcpp::PcapLiveDevice* dev, void* cookie)
{
    // extract the stats object form the cookie
    // PacketStats* stats = (PacketStats*)cookie;

    // parsed the raw packet
    pcpp::Packet parsedPacket(rawPacket);

    std:: cout << " parsed packet" << parsedPacket.getFirstLayer();
    // collect stats from packet
    // stats->consumePacket(parsedPacket);
}

