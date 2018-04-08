cmake_minimum_required(VERSION 2.8)
project(ib_project)

# configuration values for the project
include(scripts/config.cmake)

# dependencies
include(scripts/deps.cmake)

include_directories(include)
add_subdirectory(src)
add_subdirectory(bin)
