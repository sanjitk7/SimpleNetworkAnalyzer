#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include "SystemUtils.h"
#include "PcapFileDevice.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "Packet.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "PcapLiveDeviceList.h"

struct PacketMetaData
{
    long long sampleId;
    long long timeStamp;
    long timeStampNs;
    long timeStampSecs;
    int packetSize;
    std::string sourceIP;
    std::string destIP;
    int srcPort;
    int destnPort;
    int seqNo;
    int ackNo;
    // int retransmissionCount; // offline computation
    int windowSize;

    /**
     * Clear all stats
     */
    void clear()
    {
        timeStamp = 0;
        sampleId = 0;
        packetSize = 0;
        sourceIP = "";
        destIP = "";
        srcPort = 0;
        destnPort = 0;
        seqNo = 0;
        ackNo = 0; /*retransmissionCount = 0;*/
        windowSize = 0;
    }

    /**
     * C'tor
     */
    PacketMetaData() { clear(); }

    /**
     * Collect stats from a packet
     */
    void collectStats(pcpp::Packet &packet, long long sampleId)
    {
        // std::cout << "packet collect stat" << packet.getRawPacket()->getPacketTimeStamp().tv_sec << "\n";
        this->sampleId = sampleId;
        long timeStampNs = packet.getRawPacket()->getPacketTimeStamp().tv_nsec;
        long timeStampSecs = packet.getRawPacket()->getPacketTimeStamp().tv_sec;
        std::chrono::nanoseconds timestampInNanoSecs = std::chrono::seconds(timeStampSecs) +
                                                       std::chrono::nanoseconds(timeStampNs);
        std::cout << "tsInNanosecs_collect:" << timeStampNs << ":" << timeStampSecs << "\n";
        timeStamp = timestampInNanoSecs.count();
        std::cout << "ts:" << timeStamp << "\n";

        packetSize = packet.getRawPacket()->getRawDataLen();

        pcpp::IPv4Layer *ipLayer = packet.getLayerOfType<pcpp::IPv4Layer>();
        if (ipLayer == NULL)
        {
            std::cerr << "Something went wrong, couldn't find IPv4 layer" << std::endl;
        }
        sourceIP = ipLayer->getSrcIPAddress().toString();
        destIP = ipLayer->getDstIPAddress().toString();

        pcpp::TcpLayer *tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
        if (tcpLayer == NULL)
        {
            std::cerr << "Something went wrong, couldn't find TCP layer" << std::endl;
        }
        windowSize = pcpp::netToHost16(tcpLayer->getTcpHeader()->windowSize);
        seqNo = pcpp::netToHost16(tcpLayer->getTcpHeader()->sequenceNumber);
        ackNo = pcpp::netToHost16(tcpLayer->getTcpHeader()->ackNumber);
        srcPort = pcpp::netToHost16(tcpLayer->getTcpHeader()->portSrc);
        destnPort = pcpp::netToHost16(tcpLayer->getTcpHeader()->portDst);
    }

    static std::string createHeader(pcpp::PcapLiveDevice &dev, long long sampleId, int sampleSize)
    {
        std::string interfaceName = dev.getName();
        std::string interfaceDesc = dev.getDesc();
        std::string macAddress = dev.getMacAddress().toString();
        std::string defaultGateway = dev.getDefaultGateway().toString();

        rapidjson::Document doc;
        doc.SetArray();

        // sampleId is same as timestamp
        rapidjson::Value timeStampValue(sampleId);
        rapidjson::Value sampleIdValue(sampleId);
        rapidjson::Value sampleSizeValue(sampleSize);
        rapidjson::Value interfaceNameValue(rapidjson::StringRef(interfaceName.c_str()));
        rapidjson::Value interfaceDescValue(rapidjson::StringRef(interfaceDesc.c_str()));
        rapidjson::Value macAddressValue(rapidjson::StringRef(macAddress.c_str()));
        rapidjson::Value defaultGatewayValue(rapidjson::StringRef(defaultGateway.c_str()));

        rapidjson::Value packetJSONObj(rapidjson::kObjectType);
        // packetJSONObj.AddMember("timestamp", timeStampValue, doc.GetAllocator());
        // packetJSONObj.AddMember("sampleId", sampleIdValue, doc.GetAllocator());
        // packetJSONObj.AddMember("sampleSize", sampleSizeValue, doc.GetAllocator());
        // packetJSONObj.AddMember("interfaceName", interfaceName, doc.GetAllocator());
        // packetJSONObj.AddMember("interfaceDesc", interfaceDesc, doc.GetAllocator());
        // packetJSONObj.AddMember("macAddress", macAddressValue, doc.GetAllocator());
        // packetJSONObj.AddMember("defaultGateway", defaultGatewayValue, doc.GetAllocator());

        doc.PushBack(packetJSONObj, doc.GetAllocator());

        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        doc.Accept(writer);

        std::string jsonStr = strbuf.GetString();

        return jsonStr;
    }

    /**
     * Print stats to console
     */
    void printToConsole()
    {
        std::cout

            << "-------------------------------\n"
            << "timestamp:" << timeStamp << std::endl
            << "sourceIP:" << sourceIP << std::endl
            << "destIP:" << destIP << std::endl
            // << "port:" << port << std::endl
            << "packetSize:" << packetSize << std::endl
            << "windowSize " << windowSize << std::endl
            << "seqNo:" << seqNo << std::endl
            << "ackNo:" << ackNo << std::endl
            << "-------------------------------\n";
    }

    std::string todaysDate()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        std::tm tm = *std::localtime(&t);

        char date_str[11]; // "YYYY-MM-DD\0"
        std::strftime(date_str, sizeof(date_str), "%F", &tm);
        return date_str;
    }

    std::string convertToJSON()
    {
        std::cout << "convertToJSON!\n";
        rapidjson::Document doc;
        doc.SetArray();

        rapidjson::Value timeStampLongLong(timeStamp);
        rapidjson::Value sampleUniqueId(sampleId);
        rapidjson::Value packetSizeInt(packetSize);
        rapidjson::Value seqNoInt(seqNo);
        rapidjson::Value ackNoInt(ackNo);
        rapidjson::Value windowSizeInt(windowSize);
        rapidjson::Value sourceStrValue(rapidjson::StringRef(sourceIP.c_str()));
        rapidjson::Value destStrValue(rapidjson::StringRef(destIP.c_str()));
        rapidjson::Value srcPortValue(srcPort);
        rapidjson::Value destnPortValue(destnPort);

        rapidjson::Value packetJSONObj(rapidjson::kObjectType);
        packetJSONObj.AddMember("timestamp", timeStampLongLong, doc.GetAllocator());
        packetJSONObj.AddMember("sampleId", sampleUniqueId, doc.GetAllocator());
        packetJSONObj.AddMember("sourceIP", sourceStrValue, doc.GetAllocator());
        packetJSONObj.AddMember("destIP", destStrValue, doc.GetAllocator());
        packetJSONObj.AddMember("packetSize", packetSizeInt, doc.GetAllocator());
        packetJSONObj.AddMember("seqNo", seqNoInt, doc.GetAllocator());
        packetJSONObj.AddMember("ackNo", ackNoInt, doc.GetAllocator());
        packetJSONObj.AddMember("windowSizeInt", windowSizeInt, doc.GetAllocator());
        packetJSONObj.AddMember("sourcePort", srcPortValue, doc.GetAllocator());
        packetJSONObj.AddMember("destnPort", destnPortValue, doc.GetAllocator());


        doc.PushBack(packetJSONObj, doc.GetAllocator());

        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        doc.Accept(writer);

        std::string pcapIndex = "pcap_" + todaysDate();

        std::string jsonStr = "{\"index\": {\"_index\": \"" +  pcapIndex + "\", \"_id\": \"\"}}\n";
        jsonStr += strbuf.GetString();

        // std::cout << "JSON Strong for Packet:" <<jsonStr << std::endl;
        return jsonStr;
    }

    std::string toString()
    {
        std::stringstream ss;
        ss << "PacketMetaData { timeStamp: " << timeStamp << ", packetSize: " << packetSize << " }";
        return ss.str();
    }
};