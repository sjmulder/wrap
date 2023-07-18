cmake_minimum_required(VERSION 3.23)
project(wrap C)

set(CMAKE_C_STANDARD 11)

add_executable(wrap main.c)
add_executable(sample sample.c)
add_executable(test test.c)
add_library(libwrap STATIC wrap.c)

set_target_properties(libwrap PROPERTIES OUTPUT_NAME wrap)

target_link_libraries(wrap libwrap)
target_link_libraries(sample libwrap)
target_link_libraries(test libwrap)
