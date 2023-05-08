#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include "common.h"
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include "./packetCapture/packetCapture.h"

int main(int argc, char *argv[])
{
    // interface creation
    std::string interfaceIPAddr = "192.168.1.14";

    PacketCapture packetCapture(interfaceIPAddr);
    packetCapture.doCaptureAndParse();

    // std::cout << "1st metadata sample" << metaDataSampleList.back()<<std::endl;

    return 0;
}