#include <gtest/gtest.h>
#include <texttree/node.hpp>

TEST(node, default_constructed_node_evaluates_to_false)
{
    tt::node node;
    EXPECT_FALSE(node);
}
