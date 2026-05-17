cmake_minimum_required(VERSION 3.8.2)
find_package(Zephyr)
project(lauflicht-input)

target_sources(app PRIVATE src/main.c) # app is defined in zephyr/cmake/modules/kernel.cmake
