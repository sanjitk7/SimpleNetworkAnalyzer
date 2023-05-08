#include <gtest/gtest.h>
#include "PcapLiveDevice.h"
#include "../../packetCapture/packetCapture.h"

// Demonstrate some basic assertions.
TEST(DeviceTest, deviceSuccessAssertion)
{

  // positive case
  std::string interfaceIPAddr1 = "192.168.1.14";
  PacketCapture packetCapture1(interfaceIPAddr1);
  pcpp::PcapLiveDevice *dev1 = packetCapture1.openDevice();
  EXPECT_EQ(dev1->getIPv4Address().toString(), "192.168.1.14");
}

TEST(DeviceTest, deviceFailAssertion)
{
  // negative case
  std::string interfaceIPAddr2 = "192.16222348.1.14";
  PacketCapture packetCapture2(interfaceIPAddr2);
  EXPECT_ANY_THROW(packetCapture2.openDevice());
}