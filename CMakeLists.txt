cmake_minimum_required(VERSION 2.8)
set(CMAKE_INSTALL_PREFIX /usr)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
project(whisper)

execute_process(COMMAND git submodule init)
execute_process(COMMAND git submodule update)
execute_process(COMMAND cmake . WORKING_DIRECTORY jnxlibc)
execute_process(COMMAND make . WORKING_DIRECTORY jnxlibc)
execute_process(COMMAND sudo make install . WORKING_DIRECTORY jnxlibc)

option(RUN_TESTS "TEST JNXLIBC" OFF)

if(RUN_TESTS)
  MESSAGE(STATUS "Testing jnxlibc...")
  execute_process(COMMAND ./run_tests WORKING_DIRECTORY test)
endif()

file(GLOB SRC "src/**/*.c")
add_executable(whisper ${SRC})
install(TARGETS whisper DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
target_link_libraries(whisper -ljnxc -lcrypto -lssl -g -lprotobuf-c -luuid)
