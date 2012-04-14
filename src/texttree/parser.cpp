#include <texttree/parser.hpp>
#include <texttree/node.hpp>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>

namespace tt {

syntax_error::syntax_error(std::string const& what) : std::runtime_error(what)
{
}

namespace
{
    struct parse_node
    {
        tt::node_ptr node;
        bool inside_parens;
    };

    void text(std::vector<parse_node>& stack, std::string const& string)
    {
        node_ptr child = std::make_shared<node>(string);

        if (stack.back().inside_parens)
        {
            stack.back().node->append_child(child);
            stack.push_back({child, false});
        }
        else
        {
            stack.pop_back();
            stack.back().node->append_child(child);
        }
    }
}

node_ptr const parse_children(std::string const& string)
{
    std::vector<parse_node> stack;
    stack.push_back({std::make_shared<node>(), true});

    for (auto iterator = begin(string); iterator != end(string);)
    {
        if (*iterator == '\"')
        {
            ++iterator;
            auto end_of_string = std::find_if(iterator, end(string), [](char c)
            {
                return c == '\"';
            });

            if (end_of_string == end(string))
            {
                throw syntax_error("Unterminated quoted string");
            }

            text(stack, std::string(iterator, end_of_string));
            iterator = end_of_string;
        }
        else if (*iterator == '(')
        {
            if (stack.back().inside_parens)
            {
                throw syntax_error("Unexpected (");
            }

            iterator = std::find_if(iterator, end(string), [](char c)
            {
                return c == ')';
            });

            if (iterator == end(string))
            {
                throw syntax_error("Missing closing paren");
            }
        }
        else if (*iterator == ')')
        {
            throw syntax_error("Unexpected )");
        }
        else if (!isspace(*iterator))
        {
            auto end_of_string = std::find_if(iterator, end(string), [](char c)
            {
                if (c == '\"')
                {
                    throw syntax_error("Found \" in an unquoted string");
                }

                return isspace(c) || c == '(' || c == ')';
            });

            text(stack, std::string(iterator, end_of_string));
            iterator = end_of_string;
            continue;
        }

        ++iterator;
    }

    assert(stack.size() <= 2);
    return stack.front().node;
}

node_ptr const parse(std::string const& string)
{
    node_ptr node = parse_children(string);
    return node->child_count() > 0 ? node->child_at(0) : node_ptr();
}

} // namespace tt
