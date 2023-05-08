cmake_minimum_required(VERSION 3.14)
project(SimpleNetworkAnalysisTool)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  packetCaptureTest
  PacketCaptureTest.cpp
)
target_link_libraries(
  packetCaptureTest
  /Users/kumarmuthusamy/dev/aviz/SimpleNetworkAnalyzerTool/packetCapture/packetCapture.o
  /usr/local/lib/libPcap++.a
  /usr/local/lib/libPacket++.a			
  /usr/local/lib/libCommon++.a
  -lpcap
  -lpthread
  "-framework SystemConfiguration"
  "-framework CoreFoundation"
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(packetCaptureTest)

target_include_directories(packetCaptureTest PRIVATE
  include
  /usr/local/include/pcapplusplus/
  ../../dependencies/include
)

enable_testing()

add_executable(
  devicesTest
  devicesTest.cpp
)
target_link_libraries(
  devicesTest
  /Users/kumarmuthusamy/dev/aviz/SimpleNetworkAnalyzerTool/packetCapture/packetCapture.o
  /usr/local/lib/libPcap++.a
  /usr/local/lib/libPacket++.a			
  /usr/local/lib/libCommon++.a
  -lpcap
  -lpthread
  "-framework SystemConfiguration"
  "-framework CoreFoundation"
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(devicesTest)

target_include_directories(devicesTest PRIVATE
  include
  /usr/local/include/pcapplusplus/
  ../../dependencies/include
)