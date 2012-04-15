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

void node::set_text(std::string const& text)
{
    text_ = text;
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

node::iterator const node::begin()
{
    return children_.begin();
}

node::const_iterator const node::begin() const
{
    return children_.begin();
}

node::iterator const node::end()
{
    return children_.end();
}

node::const_iterator const node::end() const
{
    return children_.end();
}

node_ptr const find(node const& node, std::string const& text)
{
    for (auto const& child : node)
    {
        if (child->text() == text)
        {
            return child;
        }
    }

    return {};
}

} // namespace tt
