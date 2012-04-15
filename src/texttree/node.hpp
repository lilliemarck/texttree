#ifndef TEXTTREE_NODE_HPP
#define TEXTTREE_NODE_HPP

#include <memory>
#include <string>
#include <vector>

namespace tt {

struct node
{
    node();
    node(std::string const& text);

    std::string text;
    std::vector<node> children;
};

node const* const find(node const& node, std::string const& text);

} // namespace tt

#endif // TEXTTREE_NODE_HPP
