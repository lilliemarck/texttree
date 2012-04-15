#include <gtest/gtest.h>
#include <texttree/tree_builder.hpp>

tt::node_ptr const parse_one(std::string const& string)
{
    auto node = tt::parse_children(string);
    return node->child_count() > 0 ? node->child_at(0) : nullptr;
}

TEST(parser, parse_empty_string)
{
    tt::node_ptr node = parse_one(std::string());
    EXPECT_FALSE(node);
}

TEST(parser, parse_unquoted_string)
{
    tt::node_ptr node = parse_one("submarine");
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("submarine", node->text());
}

TEST(parser, unquoted_string_surrounded_by_whitespace)
{
    tt::node_ptr node = parse_one(" \ndata\t");
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
    tt::node_ptr node = parse_one("\"Pholiderpeton\"");
    EXPECT_EQ("Pholiderpeton", node->text());
}

TEST(parser, throws_on_unterminated_string)
{
    ASSERT_THROW(parse_one("\"inch"), tt::syntax_error);
}

TEST(parser, throws_on_quote_in_unquoted_string)
{
    ASSERT_THROW(parse_one("beetle\""), tt::syntax_error);
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

TEST(parser, unqoted_string_followed_by_empty_parens)
{
    tt::node_ptr node = parse_one("peace()");
    EXPECT_EQ("peace", node->text());
    EXPECT_EQ(0u, node->child_count());
}

TEST(parser, quoted_string_followed_by_empty_parens)
{
    tt::node_ptr node = parse_one("\"healthy\"()");
    EXPECT_EQ("healthy", node->text());
    EXPECT_EQ(0u, node->child_count());
}

TEST(parser, syntax_error_on_missing_closing_paren)
{
    ASSERT_THROW(parse_one("reviewer("), tt::syntax_error);
}

TEST(parser, syntax_error_on_only_open_paren)
{
    ASSERT_THROW(parse_one("("), tt::syntax_error);
}

TEST(parser, syntax_error_on_unexpected_closing_paren)
{
    ASSERT_THROW(parse_one(")"), tt::syntax_error);
}

TEST(parser, syntax_error_on_unexpected_closing_paren_after_text)
{
    ASSERT_THROW(parse_one("chicken)"), tt::syntax_error);
}

TEST(parser, syntax_error_on_opening_paren_after_closing_paren)
{
    ASSERT_THROW(parse_one("launch()("), tt::syntax_error);
}

TEST(parser, parse_hiearchy)
{
    tt::node_ptr keen = parse_one("keen(fence(revise) without)");
    EXPECT_EQ("keen", keen->text());
    ASSERT_EQ(2u, keen->child_count());

    tt::node_ptr fence = keen->child_at(0);
    EXPECT_EQ("fence", fence->text());
    ASSERT_EQ(1u, fence->child_count());

    tt::node_ptr revise = fence->child_at(0);
    EXPECT_EQ("revise", revise->text());
    ASSERT_EQ(0u, revise->child_count());

    tt::node_ptr without = keen->child_at(1);
    EXPECT_EQ("without", without->text());
    ASSERT_EQ(0u, without->child_count());
}

TEST(parser, parse_hiearchy_with_more_whitespace_and_quoted_strings)
{
    tt::node_ptr keen = parse_one("keen   (fence\n(revise\t) \"with out\")");
    EXPECT_EQ("keen", keen->text());
    ASSERT_EQ(2u, keen->child_count());

    tt::node_ptr fence = keen->child_at(0);
    EXPECT_EQ("fence", fence->text());
    ASSERT_EQ(1u, fence->child_count());

    tt::node_ptr revise = fence->child_at(0);
    EXPECT_EQ("revise", revise->text());
    ASSERT_EQ(0u, revise->child_count());

    tt::node_ptr with_out = keen->child_at(1);
    EXPECT_EQ("with out", with_out->text());
    ASSERT_EQ(0u, with_out->child_count());
}

TEST(parser, escapes_in_quoted_string)
{
    tt::node_ptr node = parse_one("\"\\\"\\\\\"");
    EXPECT_EQ("\"\\", node->text());
}

TEST(parser, parse_error_on_unsupported_esacpe)
{
    ASSERT_THROW(parse_one("\"\\t\""), tt::syntax_error);
}

TEST(parser, syntax_error_on_eof_in_nested_unqoted_string)
{
    ASSERT_THROW(parse_one("daze(monsoon"), tt::syntax_error);
}
