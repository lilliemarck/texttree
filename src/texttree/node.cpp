#include <texttree/node.hpp>

namespace tt {

node::node()
{
}

node::node(std::string const& text) : text(text)
{
}

node_ptr const find(node const& node, std::string const& text)
{
    for (auto const& child : node.children)
    {
        if (child->text == text)
        {
            return child;
        }
    }

    return {};
}

} // namespace tt
