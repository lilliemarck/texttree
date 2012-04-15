cmake_minimum_required(VERSION 2.8)
project(TextTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic -Werror")

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
