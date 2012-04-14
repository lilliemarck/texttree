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

TEST(parser, quoted_string)
{
    tt::node_ptr node = tt::parse("\"Pholiderpeton\"");
    EXPECT_EQ("Pholiderpeton", node->text());
}

TEST(parser, throws_on_unterminated_string)
{
    ASSERT_THROW(tt::parse("\"inch"), tt::syntax_error);
}

TEST(parser, throws_on_quote_in_unquoted_string)
{
    ASSERT_THROW(tt::parse("beetle\""), tt::syntax_error);
}

TEST(parser, space_separated_quoted_strings)
{
    tt::node_ptr nodes = tt::parse_children("\"system\" \"best\"");
    ASSERT_EQ(2u, nodes->child_count());
    EXPECT_TRUE(nodes->text().empty());
    EXPECT_EQ("system", nodes->child_at(0)->text());
    EXPECT_EQ("best", nodes->child_at(1)->text());
}

TEST(parser, non_space_separated_quoted_strings)
{
    tt::node_ptr nodes = tt::parse_children("\"pirate\"\"emission\"");
    ASSERT_EQ(2u, nodes->child_count());
    EXPECT_TRUE(nodes->text().empty());
    EXPECT_EQ("pirate", nodes->child_at(0)->text());
    EXPECT_EQ("emission", nodes->child_at(1)->text());
}
