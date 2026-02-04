cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
cmake_policy(VERSION 3.31)

if(NOT DEFINED ENV{VCPKG_ROOT})
	message(FATAL_ERROR "VCPKG_ROOT is not defined!")
else()
	message("VCPKG_ROOT is defined as $ENV{VCPKG_ROOT}")
endif()

# Those options may be set in the preset file
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

project(two_queues DESCRIPTION "Processing two queues." VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(
	$<$<CXX_COMPILER_ID:MSVC>:/we4715>
	$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror=return-type>
)

add_library(pch STATIC src/pch/dummy_pch.cpp)
target_precompile_headers(pch PUBLIC src/pch/pch.hpp)

file(GLOB cpp_files CONFIGURE_DEPENDS src/*.cpp)
file(GLOB hpp_files CONFIGURE_DEPENDS src/*.hpp)

add_executable(two_queues ${hpp_files} ${cpp_files})
target_precompile_headers(two_queues REUSE_FROM pch)

find_package(boost_json CONFIG REQUIRED)
target_link_libraries(two_queues PRIVATE Boost::json)
