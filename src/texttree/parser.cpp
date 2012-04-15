#include <texttree/parser.hpp>
#include <texttree/node.hpp>
#include <cassert>
#include <cctype>

namespace tt {

syntax_error::syntax_error(std::string const& what) : std::runtime_error(what)
{
}

parser::parser(parser_delegate& delegate) :
    delegate_(delegate),
    state_(whitespace)
{
    is_open_.push(true);
}

void parser::parse(char const* begin, char const* end)
{
    char const* iterator = begin;

    while (iterator < end)
    {
        char c = *iterator;

        switch (state_)
        {
            case whitespace:
                if (c == '(')
                {
                    if (is_open_.top())
                    {
                        throw syntax_error("Unexpected (");
                    }
                    is_open_.top() = true;
                }
                else if (c == ')')
                {
                    if (!is_open_.top())
                    {
                        end_node();
                    }

                    if (is_open_.size() == 1)
                    {
                        throw syntax_error("Unexpected )");
                    }

                    if (is_open_.top())
                    {
                        end_node();
                    }
                }
                else if (c == '\"')
                {
                    state_ = quoted_text;
                }
                else if (!isspace(c))
                {
                    state_ = unquoted_text;
                    continue;
                }
                break;

            case unquoted_text:
                if (c == '\"')
                {
                    throw syntax_error("Found \" in an unquoted string");
                }
                else if (isspace(c) || c == '(' || c == ')')
                {
                    begin_node();
                    state_ = whitespace;
                    continue;
                }
                else
                {
                    buffer_ += c;
                }
                break;

            case quoted_text:
                if (c == '\"')
                {
                    begin_node();
                    state_ = whitespace;
                }
                else if (c == '\\')
                {
                    state_ = escape_character;
                }
                else
                {
                    buffer_ += c;
                }
                break;

            case escape_character:
                if (c == '\"' || c == '\\')
                {
                    buffer_ += c;
                    state_ = quoted_text;
                }
                else
                {
                    throw syntax_error("Unknown escaped character");
                }
                break;

            default:
                assert(false);
                break;
        }

        ++iterator;
    }
}

void parser::end_parse()
{
    if (state_ == unquoted_text)
    {
        begin_node();
    }
    else if (state_ == quoted_text)
    {
        throw syntax_error("Unterminated quoted string");
    }

    if ((is_open_.size() > 2) ||
        (is_open_.size() == 2 && is_open_.top()))
    {
        throw syntax_error("Missing )");
    }

    if (is_open_.size() == 2 && !is_open_.top())
    {
        end_node();
    }
}

void parser::begin_node()
{
    if (!is_open_.top())
    {
        end_node();
    }

    is_open_.push(false);
    delegate_.begin_node(buffer_);
    buffer_.clear();
}

void parser::end_node()
{
    is_open_.pop();
    delegate_.end_node();
}

namespace
{
    class tree_builder : public parser_delegate
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
}

node_ptr const parse_children(std::string const& string)
{
    tree_builder builder;
    parser parser(builder);
    parser.parse(&*begin(string), &*end(string));
    parser.end_parse();
    return builder.tree();
}

node_ptr const parse(std::string const& string)
{
    node_ptr node = parse_children(string);
    return node->child_count() > 0 ? node->child_at(0) : node_ptr();
}

} // namespace tt
