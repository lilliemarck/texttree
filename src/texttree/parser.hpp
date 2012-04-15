#ifndef TEXT_TREE_PARSER_HPP
#define TEXT_TREE_PARSER_HPP

#include <texttree/node.hpp>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

namespace tt {

class syntax_error : public std::runtime_error
{
public:
    syntax_error(std::string const& what);
};

class parser_delegate
{
public:
    virtual void begin_node(std::string const& text) = 0;
    virtual void end_node() = 0;
};

class parser
{
public:
    parser(parser_delegate& delegate);
    void parse(char const *begin, char const *end);
    void end_parse();

private:
    void begin_node();
    void end_node();

    enum state
    {
        whitespace,
        unquoted_text,
        quoted_text,
        escape_character
    };

    parser_delegate& delegate_;
    state state_;
    std::stack<bool,std::vector<bool>> is_open_;
    std::string buffer_;
};

node_ptr const parse_children(std::string const& string);
node_ptr const parse(std::string const& string);

} // namespace tt

#endif // TEXT_TREE_PARSER_HPP
