#ifndef TEXT_TREE_PARSER_HPP
#define TEXT_TREE_PARSER_HPP

#include <texttree/node.hpp>

namespace tt {

node_ptr parse_children(std::string const& string);
node_ptr parse(std::string const& string);

} // namespace tt

#endif // TEXT_TREE_PARSER_HPP
