cmake_minimum_required(VERSION 2.8)
set(CMAKE_INSTALL_PREFIX /usr)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
project(whisper)

file(GLOB SRC "src/**/*.c")
add_executable(whisper ${SRC})
install(TARGETS whisper DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
target_link_libraries(whisper -ljnxc)
