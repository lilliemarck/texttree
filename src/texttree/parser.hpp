#ifndef TEXT_TREE_PARSER_HPP
#define TEXT_TREE_PARSER_HPP

#include <texttree/node.hpp>

namespace tt {

node_ptr const parse_children(std::string const& string);
node_ptr const parse(std::string const& string);

} // namespace tt

#endif // TEXT_TREE_PARSER_HPP
