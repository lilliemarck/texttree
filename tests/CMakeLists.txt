find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test
    node_test.cpp
    parser_test.cpp)

target_link_libraries (test ${GTEST_BOTH_LIBRARIES} pthread texttree)
