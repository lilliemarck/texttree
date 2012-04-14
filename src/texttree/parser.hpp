#ifndef TEXT_TREE_PARSER_HPP
#define TEXT_TREE_PARSER_HPP

#include <texttree/node.hpp>
#include <stdexcept>

namespace tt {

class syntax_error : public std::runtime_error
{
public:
    syntax_error(std::string const& what);
};

node_ptr const parse_children(std::string const& string);
node_ptr const parse(std::string const& string);

} // namespace tt

#endif // TEXT_TREE_PARSER_HPP
