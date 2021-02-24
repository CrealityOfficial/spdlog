cmake_minimum_required (VERSION 3.1)
project(spdlog)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigureAllModules)   

add_subdirectory(src)
add_subdirectory(demo)