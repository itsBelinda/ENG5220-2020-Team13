cmake_minimum_required(VERSION 3.9)
project(BeeSafePI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

add_compile_options(--coverage -O0)

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

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
