cmake_minimum_required(VERSION 3.9)
project(BeeSafePI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)

# Include OpenSLL for cpprestsdk; may be platform dependent.
set(OPENSSL_INCLUDE_DIR /usr/lib/ssl)
set(CMAKE_PREFIX_PATH /usr/lib/arm-linux-gneuabihg/cmake/)



# TODO: Implement conditionals for the Linux, windows, and apple.

find_package(cpprestsdk REQUIRED)

add_executable(BeeSafePI src/Main.cpp src/geo/Fence.h src/geo/RoundFence.cpp src/geo/PolyFence.cpp src/contact/Contact.cpp src/contact/Contact.h src/geo/Fence.cpp src/geo/RoundFence.h src/geo/PolyFence.h src/device/Account.cpp src/device/Account.h src/device/AccountBuilder.cpp src/device/AccountBuilder.h src/monitor/Monitor.cpp src/monitor/Monitor.h)
target_link_libraries(BeeSafePI PRIVATE cpprestsdk::cpprest)
