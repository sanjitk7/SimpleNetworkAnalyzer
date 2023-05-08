#include <gtest/gtest.h>
#include "PcapLiveDevice.h"
#include "../../packetCapture/packetCapture.h"

// Demonstrate some basic assertions.
TEST(PacketCaptureTest, basicCaptureAssersions) {

  std::string interfaceIPAddr = "192.168.1.14";

  PacketCapture packetCapture(interfaceIPAddr);
  // packetCapture.doCaptureAndParse();
  pcpp::PcapLiveDevice *dev = packetCapture.openDevice();
  
  EXPECT_EQ(dev->getIPv4Address().toString(), "192.168.1.14");
  
}