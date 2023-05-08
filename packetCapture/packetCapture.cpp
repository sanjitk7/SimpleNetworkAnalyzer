#include "PcapLiveDeviceList.h"
#include "PcapFilter.h"
#include "packetCapture.h"
#include "packetCaptureCallback.h"
#include "SystemUtils.h"
#include "../common.h"
#include <string>
#include <list>

pcpp::AndFilter *PacketCapture::addFilters()
{
    pcpp::PortFilter portFilter(80, pcpp::SRC_OR_DST);

    // create a filter instance to capture only TCP traffic
    pcpp::ProtoFilter protocolFilter(pcpp::TCP);

    // create an AND filter to combine both filters - capture only TCP traffic on port 80
    pcpp::AndFilter *andFilter = new pcpp::AndFilter();
    andFilter->addFilter(&portFilter);
    andFilter->addFilter(&protocolFilter);

    return andFilter;
}

pcpp::PcapLiveDevice *PacketCapture::openDevice()
{
    pcpp::PcapLiveDevice *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr);

    if (dev == NULL)
    {
        std::cerr << "Cannot find interface with IPv4 address of '" << interfaceIPAddr << "'" << std::endl;
        throw "Cannot find interface exception!";
    }

    // open the device before start capturing/sending packets
    if (!dev->open())
    {
        std::cerr << "Cannot open device" << std::endl;
        throw "Cannot open device";
    }

    return dev;
}

// pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie

// void PacketCapture::doCapture(std::function<void()> startCapture)
std::list<std::string> PacketCapture::doCaptureAndParse()
{
    int result = performSomeAction();
    std::cout << "ipv4 interface addr: " << interfaceIPAddr << " and result: " << result << "\n";

    try
    {
        pcpp::PcapLiveDevice *dev = openDevice();
        std::cout << "opened device ipv4: " << dev->getIPv4Address() << "\n";
        // pcpp::IPcapDevice::PcapStats stats;

        PacketStats stats;
        pcpp::RawPacketVector packetVec;

        dev->startCapture(packetVec);

        // // // Add Sourced and Protocol Filters
        // pcpp::AndFilter andFilter = addFilters();
        // dev-> setFilter(andFilter);

        // create a filter instance to capture only traffic on port 80
        pcpp::PortFilter portFilter(80, pcpp::SRC_OR_DST);

        // create a filter instance to capture only TCP traffic
        pcpp::ProtoFilter protocolFilter(pcpp::TCP);

        // create an AND filter to combine both filters - capture only TCP traffic on port 80
        pcpp::AndFilter andFilter;
        andFilter.addFilter(&portFilter);
        andFilter.addFilter(&protocolFilter);

        // set the filter on the device
        // dev->setFilter(*addFilters());
        dev->setFilter(andFilter);

        pcpp::multiPlatformSleep(10);
        dev->stopCapture();

        // Calcualte sampelId
        long long sampleId = 0;
        std::list<std::string> jsonStringList;

        // Log device info
        // createHeader(pcpp::PcapLiveDevice dev, long long sampleId, int sampleSize)
        pcpp::Packet parsedPacket(packetVec.front());
        long timeStampNs = parsedPacket.getRawPacket()->getPacketTimeStamp().tv_nsec;
        long timeStampSecs = parsedPacket.getRawPacket()->getPacketTimeStamp().tv_sec;
        std::chrono::nanoseconds timestampInNanoSecs = std::chrono::seconds(timeStampSecs) +
                                                       std::chrono::nanoseconds(timeStampNs);
        std::cout << "Inside tsInNanosecs_collect:" << timeStampNs << ":" << timeStampSecs << "\n";
        sampleId = timestampInNanoSecs.count();
        std::cout << "Inside ts:" << sampleId << "\n";

        std::string jsonHeader = PacketMetaData::createHeader(*dev, sampleId, packetVec.size());
        jsonStringList.push_back(jsonHeader);

        std::cout << "jsonHeader:" << jsonHeader << "\n";

            // finish capture and return to main
            // find:
            /*
            1. Timestamps
            2. Packet Size
            3. Source IP
            4. Dest IP
            5. Port
            6. Seq Number
            7. ACK no
            8. Retransmission count
            9. window size
        */

            std::list<PacketMetaData *> *packetMetaDataList = new std::list<PacketMetaData *>();
        for (pcpp::RawPacketVector::ConstVectorIterator iter = packetVec.begin(); iter != packetVec.end(); iter++)
        {
            PacketMetaData *data = new PacketMetaData();
            // parse raw packet
            pcpp::Packet parsedPacket(*iter);

            // if (iter == packetVec.begin())
            // {
            //     long timeStampNs = parsedPacket.getRawPacket()->getPacketTimeStamp().tv_nsec;
            //     long timeStampSecs = parsedPacket.getRawPacket()->getPacketTimeStamp().tv_sec;
            //     std::chrono::nanoseconds timestampInNanoSecs = std::chrono::seconds(timeStampSecs) +
            //                                                    std::chrono::nanoseconds(timeStampNs);
            //     std::cout << "Inside tsInNanosecs_collect:" << timeStampNs << ":" << timeStampSecs << "\n";
            //     sampleId = timestampInNanoSecs.count();
            //     std::cout << "Inside ts:" << sampleId << "\n";
            // }

            // feed packet to the stats object
            data->collectStats(parsedPacket, sampleId);

            // data->printToConsole();
            jsonStringList.push_back(data->convertToJSON());

            packetMetaDataList->push_back(data);
        }

        delete packetMetaDataList;

        // std::cout << "Results:" << std::endl;
        // stats.printToConsole();
        // stats.clear();
        return jsonStringList;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // Invoke the callback function
    // startCapture(result);
}

int PacketCapture::performSomeAction()
{
    // Some logic that needs to be executed before the callback
    return 42; // Return some value to be used by the callback
}

PacketCapture::PacketCapture(std::string interfaceToSniff)
{
    interfaceIPAddr = interfaceToSniff;
}
