#ifndef TEXT_TREE_PARSER_HPP
#define TEXT_TREE_PARSER_HPP

#include <memory>

namespace tt {

typedef std::shared_ptr<class node> node_ptr;

node_ptr parse(std::string const& string);

} // namespace tt


#endif // TEXT_TREE_PARSER_HPP
