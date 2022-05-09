# Copyright(c) 2019 spdlog authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.10)

# ---------------------------------------------------------------------------------------
# Start spdlog project
# ---------------------------------------------------------------------------------------
include(cmake/utils.cmake)
include(cmake/ide.cmake)

spdlog_extract_version()

#project(spdlog VERSION ${SPDLOG_VERSION} LANGUAGES CXX)
message(STATUS "Build spdlog: ${SPDLOG_VERSION}")

include(GNUInstallDirs)

# ---------------------------------------------------------------------------------------
# Set CMake policies to support later version behaviour
# ---------------------------------------------------------------------------------------
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW) # option() honors variables already set
endif()

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

# make sure __cplusplus is defined when using msvc and enable parallel build
if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS")
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

# ---------------------------------------------------------------------------------------
# Set SPDLOG_MASTER_PROJECT to ON if we are building spdlog
# ---------------------------------------------------------------------------------------
# Check if spdlog is being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED SPDLOG_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SPDLOG_MASTER_PROJECT ON)
    else()
        set(SPDLOG_MASTER_PROJECT OFF)
    endif()
endif()

option(SPDLOG_BUILD_ALL "Build all artifacts" OFF)

# build shared option
option(SPDLOG_BUILD_SHARED "Build shared library" ON)

# precompiled headers option
option(SPDLOG_ENABLE_PCH "Build static or shared library using precompiled header to speed up compilation time" OFF)

# example options
option(SPDLOG_BUILD_EXAMPLE "Build example" ${SPDLOG_MASTER_PROJECT})
option(SPDLOG_BUILD_EXAMPLE_HO "Build header only example" OFF)

# testing options
option(SPDLOG_BUILD_TESTS "Build tests" OFF)
option(SPDLOG_BUILD_TESTS_HO "Build tests using the header only version" OFF)

# bench options
option(SPDLOG_BUILD_BENCH "Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)" OFF)

# sanitizer options
option(SPDLOG_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)

# warning options
option(SPDLOG_BUILD_WARNINGS "Enable compiler warnings" OFF)

# install options
option(SPDLOG_INSTALL "Generate the install target" ${SPDLOG_MASTER_PROJECT})
option(SPDLOG_FMT_EXTERNAL "Use external fmt library instead of bundled" OFF)
option(SPDLOG_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" OFF)
option(SPDLOG_NO_EXCEPTIONS "Compile with -fno-exceptions. Call abort() on any spdlog exceptions" ON)

if(SPDLOG_FMT_EXTERNAL AND SPDLOG_FMT_EXTERNAL_HO)
    message(FATAL_ERROR "SPDLOG_FMT_EXTERNAL and SPDLOG_FMT_EXTERNAL_HO are mutually exclusive")
endif()

# misc tweakme options
if(WIN32)
    option(SPDLOG_WCHAR_SUPPORT "Support wchar api" OFF)
    option(SPDLOG_WCHAR_FILENAMES "Support wchar filenames" OFF)
else()
    set(SPDLOG_WCHAR_SUPPORT OFF CACHE BOOL "non supported option" FORCE)
    set(SPDLOG_WCHAR_FILENAMES OFF CACHE BOOL "non supported option" FORCE)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    option(SPDLOG_CLOCK_COARSE "Use CLOCK_REALTIME_COARSE instead of the regular clock," OFF)
else()
    set(SPDLOG_CLOCK_COARSE OFF CACHE BOOL "non supported option" FORCE)
endif()

option(SPDLOG_PREVENT_CHILD_FD "Prevent from child processes to inherit log file descriptors" OFF)
option(SPDLOG_NO_THREAD_ID "prevent spdlog from querying the thread id on each log call if thread id is not needed" OFF)
option(SPDLOG_NO_TLS "prevent spdlog from using thread local storage" OFF)
option(
    SPDLOG_NO_ATOMIC_LEVELS
    "prevent spdlog from using of std::atomic log levels (use only if your code never modifies log levels concurrently"
    OFF)
option(SPDLOG_DISABLE_DEFAULT_LOGGER "Disable default logger creation" OFF)

# clang-tidy
if(${CMAKE_VERSION} VERSION_GREATER "3.5")
    option(SPDLOG_TIDY "run clang-tidy" OFF)
endif()

if(SPDLOG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Enabled clang-tidy")
endif()

find_package(Threads REQUIRED)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
# ---------------------------------------------------------------------------------------
# Static/Shared library (shared not supported in windows yet)
# ---------------------------------------------------------------------------------------
set(SPDLOG_SRCS src/spdlog.cpp src/stdout_sinks.cpp src/color_sinks.cpp
				src/file_sinks.cpp src/async.cpp src/cfg.cpp
				src/cxlog.cpp)

if(NOT SPDLOG_FMT_EXTERNAL AND NOT SPDLOG_FMT_EXTERNAL_HO)
    list(APPEND SPDLOG_SRCS src/fmt.cpp)
endif()

set(BUILD_SHARED_LIBS OFF)

if(WIN32)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)
    list(APPEND SPDLOG_SRCS ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif()

set(SRCS ${SPDLOG_SRCS} ${SPDLOG_ALL_HEADERS})
set(DEFS SPDLOG_COMPILED_LIB CXLOG_DLL)
set(LIBS Threads::Threads)
if(NOT SPDLOG_FMT_EXTERNAL AND NOT SPDLOG_FMT_EXTERNAL_HO)
    list(APPEND CXLOG_DLL FMT_EXPORT)
endif()

# ---------------------------------------------------------------------------------------
# Add required libraries for Android CMake build
# ---------------------------------------------------------------------------------------
if(CC_BC_ANDROID)
    list(APPEND LIBS log)
endif()
	
if(BUILD_CXLOG_STATIC)
    __add_real_target(cxlog lib SOURCE ${SRCS}
                             DEF ${DEFS}
                             LIB ${LIBS}
                             INC ${CMAKE_CURRENT_SOURCE_DIR}/include/
                             INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/
                             )
else()
    __add_real_target(cxlog dll SOURCE ${SRCS}
                             DEF ${DEFS}
                             LIB ${LIBS}
                             INC ${CMAKE_CURRENT_SOURCE_DIR}/include/
                             INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/
                             )
endif()


spdlog_enable_warnings(cxlog)

# ---------------------------------------------------------------------------------------
# Misc definitions according to tweak options
# ---------------------------------------------------------------------------------------
set(SPDLOG_WCHAR_TO_UTF8_SUPPORT ${SPDLOG_WCHAR_SUPPORT})
foreach(
    SPDLOG_OPTION
    SPDLOG_WCHAR_TO_UTF8_SUPPORT
    SPDLOG_WCHAR_FILENAMES
    SPDLOG_NO_EXCEPTIONS
    SPDLOG_CLOCK_COARSE
    SPDLOG_PREVENT_CHILD_FD
    SPDLOG_NO_THREAD_ID
    SPDLOG_NO_TLS
    SPDLOG_NO_ATOMIC_LEVELS
    SPDLOG_DISABLE_DEFAULT_LOGGER)
    if(${SPDLOG_OPTION})
        target_compile_definitions(cxlog PRIVATE ${SPDLOG_OPTION})
    endif()
endforeach()

if(SPDLOG_NO_EXCEPTIONS AND NOT MSVC)
    target_compile_options(cxlog PRIVATE -fno-exceptions)
endif()

if(CC_INSTALL_SPDLOG)
	set(INSTALL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/spdlog/cxlog.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/spdlog/cxlog_macro.h
						)
	
	INSTALL(FILES ${INSTALL_HEADERS} DESTINATION include/spdlog/spdlog/)
endif()
