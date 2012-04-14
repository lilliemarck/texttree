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

TEST(parser, unqoted_strings_separated_by_whitespace)
{
    tt::node_ptr nodes = tt::parse_children("academy ward");
    ASSERT_TRUE(nodes != nullptr);
    ASSERT_EQ(2u, nodes->child_count());
    EXPECT_TRUE(nodes->text().empty());
    EXPECT_EQ("academy", nodes->child_at(0)->text());
    EXPECT_EQ("ward", nodes->child_at(1)->text());
}
