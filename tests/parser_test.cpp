#include <gtest/gtest.h>
#include <texttree/node.hpp>
#include <texttree/parser.hpp>

TEST(parser, parse_empty_string)
{
    tt::node_ptr node = tt::parse(std::string());
    EXPECT_FALSE(node);
}

TEST(parser, unquoted_string)
{
    tt::node_ptr node = tt::parse("submarine");
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("submarine", node->text());
}
