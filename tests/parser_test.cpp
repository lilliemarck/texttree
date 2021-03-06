#include <gtest/gtest.h>
#include <texttree/tree_builder.hpp>

typedef std::unique_ptr<tt::node> optional_node;

tt::node parse_one(std::string const& string)
{
    auto node = tt::load(string);
    assert(!node.children.empty());
    return std::move(node.children.front());
}

TEST(parser, parse_empty_string)
{
    auto node = tt::load(std::string());
    EXPECT_TRUE(node.text.empty());
}

TEST(parser, parse_unquoted_string)
{
    auto node = parse_one("submarine");
    ASSERT_EQ("submarine", node.text);
}

TEST(parser, unquoted_string_surrounded_by_whitespace)
{
    auto node = parse_one(" \ndata\t");
    ASSERT_EQ("data", node.text);
}

TEST(parser, unqoted_strings_separated_by_whitespace)
{
    tt::node nodes = tt::load("academy ward");
    ASSERT_EQ(2u, nodes.children.size());
    EXPECT_TRUE(nodes.text.empty());
    EXPECT_EQ("academy", nodes.children[0].text);
    EXPECT_EQ("ward", nodes.children[1].text);
}

TEST(parser, quoted_string)
{
    auto node = parse_one("\"Pholiderpeton\"");
    EXPECT_EQ("Pholiderpeton", node.text);
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
    tt::node nodes = tt::load("\"system\" \"best\"");
    ASSERT_EQ(2u, nodes.children.size());
    EXPECT_TRUE(nodes.text.empty());
    EXPECT_EQ("system", nodes.children[0].text);
    EXPECT_EQ("best", nodes.children[1].text);
}

TEST(parser, non_space_separated_quoted_strings)
{
    tt::node nodes = tt::load("\"pirate\"\"emission\"");
    ASSERT_EQ(2u, nodes.children.size());
    EXPECT_TRUE(nodes.text.empty());
    EXPECT_EQ("pirate", nodes.children[0].text);
    EXPECT_EQ("emission", nodes.children[1].text);
}

TEST(parser, unqoted_string_followed_by_empty_parens)
{
    auto node = parse_one("peace()");
    EXPECT_EQ("peace", node.text);
    EXPECT_EQ(0u, node.children.size());
}

TEST(parser, quoted_string_followed_by_empty_parens)
{
    auto node = parse_one("\"healthy\"()");
    EXPECT_EQ("healthy", node.text);
    EXPECT_EQ(0u, node.children.size());
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
    auto keen = parse_one("keen(fence(revise) without)");
    EXPECT_EQ("keen", keen.text);
    ASSERT_EQ(2u, keen.children.size());

    auto const& fence = keen.children[0];
    EXPECT_EQ("fence", fence.text);
    ASSERT_EQ(1u, fence.children.size());

    auto const& revise = fence.children[0];
    EXPECT_EQ("revise", revise.text);
    ASSERT_EQ(0u, revise.children.size());

    auto const& without = keen.children[1];
    EXPECT_EQ("without", without.text);
    ASSERT_EQ(0u, without.children.size());
}

TEST(parser, parse_hiearchy_with_more_whitespace_and_quoted_strings)
{
    auto keen = parse_one("keen   (fence\n(revise\t) \"with out\")");
    EXPECT_EQ("keen", keen.text);
    ASSERT_EQ(2u, keen.children.size());

    auto const& fence = keen.children[0];
    EXPECT_EQ("fence", fence.text);
    ASSERT_EQ(1u, fence.children.size());

    auto const& revise = fence.children[0];
    EXPECT_EQ("revise", revise.text);
    ASSERT_EQ(0u, revise.children.size());

    auto const& with_out = keen.children[1];
    EXPECT_EQ("with out", with_out.text);
    ASSERT_EQ(0u, with_out.children.size());
}

TEST(parser, escapes_in_quoted_string)
{
    auto node = parse_one("\"\\\"\\\\\"");
    EXPECT_EQ("\"\\", node.text);
}

TEST(parser, parse_error_on_unsupported_esacpe)
{
    ASSERT_THROW(parse_one("\"\\t\""), tt::syntax_error);
}

TEST(parser, syntax_error_on_eof_in_nested_unqoted_string)
{
    ASSERT_THROW(parse_one("daze(monsoon"), tt::syntax_error);
}
