#include <iostream>
#include <list>
#include <string>
#include "common.h"
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include "./packetCapture/packetCapture.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!";

    // interface creation
    std:: string interfaceIPAddr = "192.168.1.14";

    PacketCapture packetCapture(interfaceIPAddr);
    std::list<std::string> metaDataSampleList = packetCapture.doCaptureAndParse();
    
    std::cout << "1st metadata sample" << metaDataSampleList.back()<<std::endl;
    return 0;
}