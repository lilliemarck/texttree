#include <texttree/parser.hpp>
#include <texttree/node.hpp>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <stack>
#include <string>

namespace tt {

syntax_error::syntax_error(std::string const& what) : std::runtime_error(what)
{
}

namespace
{
    void text(std::stack<node_ptr,std::vector<node_ptr>>& stack,
              std::string::const_iterator const& begin,
              std::string::const_iterator const& end)
    {
        stack.top()->append_child(std::make_shared<node>(std::string(begin, end)));
    }
}

node_ptr const parse_children(std::string const& string)
{
    std::stack<node_ptr,std::vector<node_ptr>> stack;
    stack.push(std::make_shared<node>());

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

            text(stack, iterator, end_of_string);
            iterator = end_of_string;
        }
        else if (*iterator == '(')
        {
            auto const& parent_node = stack.top();
            size_t parent_node_child_count = parent_node->child_count();

            if (parent_node_child_count == 0)
            {
                 throw syntax_error("Unexpected (");
            }

            auto const& previous_node = parent_node->child_at(parent_node_child_count - 1);
            stack.push(previous_node);
        }
        else if (*iterator == ')')
        {
            if (stack.size() < 2)
            {
                throw syntax_error("Unexpected )");
            }

            stack.pop();
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

            text(stack, iterator, end_of_string);
            iterator = end_of_string;
            continue;
        }

        ++iterator;
    }

    if (stack.size() > 1)
    {
        throw syntax_error("Missing )");
    }

    return stack.top();
}

node_ptr const parse(std::string const& string)
{
    node_ptr node = parse_children(string);
    return node->child_count() > 0 ? node->child_at(0) : node_ptr();
}

} // namespace tt
