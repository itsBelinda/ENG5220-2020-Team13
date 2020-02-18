cmake_minimum_required(VERSION 3.9)
project(BeeSafePI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include OpenSLL for cpprestsdk; may be platform dependent.
set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/bin/openssl)

find_package(cpprestsdk REQUIRED)

add_executable(BeeSafePI src/main.cpp src/geo/Fence.h src/geo/GeoFence.cpp src/geo/GeoPath.cpp src/contact/Contact.cpp src/contact/Contact.h src/geo/Fence.cpp src/geo/GeoFence.h src/geo/GeoPath.h src/account/Account.cpp src/account/Account.h src/account/builders/AccountBuilder.cpp src/account/builders/AccountBuilder.h)
target_link_libraries(BeeSafePI PRIVATE cpprestsdk::cpprest)
