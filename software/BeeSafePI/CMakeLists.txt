cmake_minimum_required(VERSION 3.9)
project(BeeSafePI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Implement conditionals for the Linux, windows, and apple.

if(UNIX AND APPLE)
    message(STATUS "Platform: Apple")
    set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/bin/openssl)

elseif(UNIX AND NOT APPLE)
    message(STATUS "Platform: Linux")

    set(CMAKE_PREFIX_PATH /usr/lib/arm-linux-gneuabihg/cmake/)
    set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)

    # Include OpenSLL for cpprestsdk; may be platform dependent.
    set(OPENSSL_INCLUDE_DIR /usr/lib/ssl)
    set(CMAKE_PREFIX_PATH /usr/lib/arm-linux-gneuabihg/cmake/)

endif()


if(WIN32)
    message(STATUS "Platform: Win")
    set(CMAKE_PREFIX_PATH D:/Program_Files/JetBrains/CLion 2019.3.5/bin/cmake/win)
    set(CMAKE_MODULE_PATH D:/Program_Files/JetBrains/CLion 2019.3.5/bin/cmake/win/share/cmake-3.15/Modules)
    set(OPENSSL_INCLUDE_DIR D:/Program_Files/Strawberry/c/bin)
    set(cpprestsdk_DIR C:/vcpkg/packages/cpprestsdk_x86-windows)
    #set(CppRest_INCLUDE_DIR=C:/cpprestsdk/Release/include)
    #set(CppRest_LIBRARY=C:/cpprestsdk/Release/libs)
    #set(cpprestsdk_DIR C:/cpprestsdk/Release/cmake)
endif()

find_package(cpprestsdk REQUIRED)

add_executable(BeeSafePI src/Main.cpp src/geo/Fence.h src/geo/RoundFence.cpp src/geo/PolyFence.cpp src/contact/Contact.cpp src/contact/Contact.h src/geo/Fence.cpp src/geo/RoundFence.h src/geo/PolyFence.h src/device/Account.cpp src/device/Account.h src/device/AccountBuilder.cpp src/device/AccountBuilder.h src/monitor/Monitor.cpp src/monitor/Monitor.h src/monitor/states/MonitorState.cpp src/monitor/states/MonitorState.h src/monitor/states/PassiveMonitorState.cpp src/monitor/states/PassiveMonitorState.h src/monitor/states/ActiveMonitorState.cpp src/monitor/states/ActiveMonitorState.h src/comms/Comms.h src/comms/Comms.cpp src/comms/Ublox.h src/comms/Ublox.cpp src/comms/Uart.h src/comms/Uart.cpp)
target_link_libraries(BeeSafePI PRIVATE cpprestsdk::cpprest)

