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

# Threads
find_package(Threads REQUIRED)

# Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system filesystem thread)
include_directories(${Boost_INCLUDE_DIRS})

# Vendor
add_subdirectory(vendor)

# Sources
include_directories("include")
add_subdirectory(include)
add_subdirectory(src)

# GTest magic
if(TESTS)
	message(STATUS "GTest include dir: ${GTEST_INCLUDE_DIR}")
	include_directories(${GTEST_INCLUDE_DIR})
	add_custom_target(test)
	macro(run_test test_target)
		add_custom_target(${test_target}_runtest
			COMMAND ${test_target}
			DEPENDS ${test_target}
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
		add_dependencies(test ${test_target}_runtest)
	endmacro()
	add_subdirectory(tests)
endif(TESTS)

# Celero magic
if(BENCH)
	message(STATUS "Celero include dir: ${CELERO_INCLUDE_DIR}")
	include_directories(${CELERO_INCLUDE_DIR})
	add_custom_target(bench)
	macro(run_bench bench_target)
		add_custom_target(${bench_target}_runbench
			COMMAND ${bench_target}
			DEPENDS ${bench_target}
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
		add_dependencies(bench ${bench_target}_runbench)
	endmacro()
	add_subdirectory(bench)
endif(BENCH)
