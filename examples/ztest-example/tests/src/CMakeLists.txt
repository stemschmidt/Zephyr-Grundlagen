# Set minimum CMake version
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ubx_checksum)

target_include_directories(testbinary PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_sources(testbinary
    PRIVATE
    test_ubx_checksum.c
    ${CMAKE_CURRENT_LIST_DIR}/../../src/ubx_checksum.c
)
