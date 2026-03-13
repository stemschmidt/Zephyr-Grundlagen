cmake_minimum_required(VERSION 3.8.2)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mutex)

target_sources(app PRIVATE src/main.c) # app is defined in zephyr/cmake/modules/kernel.cmake
