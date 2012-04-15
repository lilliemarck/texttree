#ifndef TEXTTREE_NODE_HPP
#define TEXTTREE_NODE_HPP

#include <memory>
#include <string>
#include <vector>

namespace tt {

typedef std::shared_ptr<class node> node_ptr;

struct node
{
    node();
    node(std::string const& text);

    std::string text;
    std::vector<node_ptr> children;
};

node_ptr const find(node const& node, std::string const& text);

} // namespace tt

#endif // TEXTTREE_NODE_HPP
