#include <texttree/node.hpp>

namespace tt {

node::node()
{
}

node::node(std::string const& text) : text_(text)
{
}

std::string const node::text() const
{
    return text_;
}

void node::append_child(node_ptr const& child)
{
    children_.push_back(child);
}

size_t const node::child_count() const
{
    return children_.size();
}

node_ptr const node::child_at(size_t index) const
{
    return children_[index];
}

} // namespace tt
