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
    node const tree() const;

private:
    void begin_node(std::string const& text) override;
    void end_node() override;

    std::stack<node,std::vector<node>> stack_;
};

node const load(std::string const& string);
node const load_file(char const* filename);

} // namespace tt

#endif // TEXTTREE_TREE_BUILDER_HPP
