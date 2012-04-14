#include <gtest/gtest.h>
#include <texttree/node.hpp>

TEST(node, default_constructed_node_has_empty_string)
{
    tt::node node;
    EXPECT_TRUE(node.text().empty());
}
