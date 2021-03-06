cmake_minimum_required(VERSION 2.8)
set(CMAKE_INSTALL_PREFIX /usr)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
project(whisper)

execute_process(COMMAND git submodule init)
execute_process(COMMAND git submodule update)
execute_process(COMMAND cmake . WORKING_DIRECTORY jnxlibc)
execute_process(COMMAND make . WORKING_DIRECTORY jnxlibc)
execute_process(COMMAND sudo make install . WORKING_DIRECTORY jnxlibc)
option(RUN_TESTS "Tests" OFF)

if(RUN_TESTS)
  execute_process(COMMAND ./test/run_tests)
endif()

if(DEBUG)
  add_definitions("-g")
endif()

if(PRINT_DEBUG)
  add_definitions("-DDEBUG")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_CXX_FLAGS "-Wno-pointer-sign")
endif()
execute_process(COMMAND ./tools/generate_protobuf)
file(GLOB SRC "src/**/*.c")
include_directories(src)
add_executable(whisper ${SRC})
install(TARGETS whisper DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
target_link_libraries(whisper -ljnxc -lcrypto -lssl -g -lprotobuf-c -lncurses -lpthread)
