#include <gtest/gtest.h>
#include "PcapLiveDevice.h"
#include "../../packetCapture/packetCapture.h"
#include <iostream>
#include <string>
#include <cstring>

// using ::testing::HasSubstr;

// Demonstrate some basic assertions.
TEST(PacketCaptureTest, basicCaptureAssersions) {

  std::string interfaceIPAddr = "192.168.1.14";

  PacketCapture packetCapture(interfaceIPAddr);
  pcpp::PcapLiveDevice *dev = packetCapture.openDevice();

  std::string packetHeader = PacketMetaData::createHeader(*dev, 12030204932095034, 64);
  
  const char* found_sampleID = strstr(packetHeader.c_str(), "12030204932095034");
  const char* found_sampleSize = strstr(packetHeader.c_str(), "64");
  const char* found_recordType = strstr(packetHeader.c_str(), "H");

  EXPECT_TRUE(found_sampleID);
  EXPECT_TRUE(found_sampleSize);
  EXPECT_TRUE(found_recordType);
  
}