cmake_minimum_required(VERSION 2.8)
project(TextTree)

include_directories(src)
add_library(texttree src/texttree/node.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(test tests/node_test.cpp)
target_link_libraries (test ${GTEST_BOTH_LIBRARIES} pthread texttree)
