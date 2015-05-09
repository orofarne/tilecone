cmake_minimum_required(VERSION 2.8)

project(tilecone C CXX)

if (${CMAKE_MAJOR_VERSION} GREATER 2)
	cmake_policy(SET CMP0037 OLD)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

OPTION(TESTS "enable_testing" ON)
OPTION(BENCH "enable_benchmarks" ON)

include(CheckCXXSourceCompiles)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
	endif ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else ()
	message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# Thrift
find_package(Thrift REQUIRED)
include_directories(${THRIFT_INCLUDE_DIR})
add_subdirectory(thrift)
include_directories(${OSMDT_THRIFT_DIR})
