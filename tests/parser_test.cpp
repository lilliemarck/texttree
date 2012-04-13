#include <gtest/gtest.h>
#include <texttree/node.hpp>
#include <texttree/parser.hpp>

TEST(parser, parse_empty_string)
{
    tt::node_ptr node = tt::parse(std::string());
    EXPECT_FALSE(node);
}

TEST(parser, parse_unquoted_string)
{
    tt::node_ptr node = tt::parse("submarine");
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("submarine", node->text());
}

TEST(parser, unquoted_string_surrounded_by_whitespace)
{
    tt::node_ptr node = tt::parse(" \ndata\t");
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("data", node->text());
}
