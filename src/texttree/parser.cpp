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
    class tree_builder
    {
    public:
        tree_builder()
        {
            stack_.push(std::make_shared<node>());
        }

        void begin_node(std::string const& text)
        {
            node_ptr child = std::make_shared<node>(text);
            stack_.top()->append_child(child);
            stack_.push(child);
        }

        void end_node()
        {
            stack_.pop();
        }

        node_ptr const tree() const
        {
            assert(stack_.size() == 1);
            return stack_.top();
        }

    private:
        std::stack<node_ptr,std::vector<node_ptr>> stack_;
    };

    typedef std::stack<bool,std::vector<bool>> bool_stack;

    void text(bool_stack& is_open, tree_builder& builder,
              std::string::const_iterator const& begin,
              std::string::const_iterator const& end)
    {
        if (!is_open.top())
        {
            is_open.pop();
            builder.end_node();
        }

        is_open.push(false);
        builder.begin_node(std::string(begin, end));
    }
}

node_ptr const parse_children(std::string const& string)
{
    bool_stack is_open;
    is_open.push(true);
    tree_builder builder;

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

            text(is_open, builder, iterator, end_of_string);
            iterator = end_of_string;
        }
        else if (*iterator == '(')
        {
            if (is_open.top())
            {
                 throw syntax_error("Unexpected (");
            }

            is_open.top() = true;
        }
        else if (*iterator == ')')
        {
            if (!is_open.top())
            {
                is_open.pop();
                builder.end_node();
            }

            if (is_open.size() == 1)
            {
                throw syntax_error("Unexpected )");
            }

            if (is_open.top())
            {
                is_open.pop();
                builder.end_node();
            }
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

            text(is_open, builder, iterator, end_of_string);
            iterator = end_of_string;
            continue;
        }

        ++iterator;
    }

    if (is_open.size() >= 2 && is_open.top())
    {
        throw syntax_error("Missing )");
    }

    if (is_open.size() == 2 && !is_open.top())
    {
        builder.end_node();
    }

    return builder.tree();
}

node_ptr const parse(std::string const& string)
{
    node_ptr node = parse_children(string);
    return node->child_count() > 0 ? node->child_at(0) : node_ptr();
}

} // namespace tt
