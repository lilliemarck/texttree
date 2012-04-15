#ifndef TEXTTREE_TREE_BUILDER_HPP
#define TEXTTREE_TREE_BUILDER_HPP

#include <texttree/node.hpp>
#include <texttree/parser.hpp>
#include <stack>
#include <string>
#include <vector>

namespace tt {

class tree_builder final : public parser_delegate
{
public:
    tree_builder();
    node_ptr const tree() const;

private:
    void begin_node(std::string const& text) override;
    void end_node() override;

    std::stack<node_ptr,std::vector<node_ptr>> stack_;
};

node_ptr const parse_children(std::string const& string);
node_ptr const parse(std::string const& string);
node_ptr const parse_children_from_file(char const* filename);

} // namespace tt

#endif // TEXTTREE_TREE_BUILDER_HPP
