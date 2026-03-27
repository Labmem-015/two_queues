cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
cmake_policy(VERSION 3.31)

# Those options may be set in the preset file
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

project(two_queues DESCRIPTION "Processing two queues." VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(
	$<$<CXX_COMPILER_ID:MSVC>:/we4715>
	$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror=return-type>
)

add_library(pch STATIC pch/dummy_pch.cpp)
target_precompile_headers(pch PUBLIC pch/pch.hpp)

file(GLOB src_files CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(core STATIC ${src_files})
target_precompile_headers(core REUSE_FROM pch)
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)
target_precompile_headers(main REUSE_FROM pch)
target_link_libraries(main PRIVATE core)

add_executable(string_producer
	${PROJECT_SOURCE_DIR}/examples/string_producer.cpp
)
target_precompile_headers(string_producer REUSE_FROM pch)
target_link_libraries(string_producer PRIVATE core)


add_executable(string_consumer
	${PROJECT_SOURCE_DIR}/examples/string_consumer.cpp
)
target_precompile_headers(string_consumer REUSE_FROM pch)
target_link_libraries(string_consumer PRIVATE core)


add_executable(string_processor
	${PROJECT_SOURCE_DIR}/examples/string_processor.cpp
)
target_precompile_headers(string_processor REUSE_FROM pch)
target_link_libraries(string_processor PRIVATE core)