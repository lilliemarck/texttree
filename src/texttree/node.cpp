#include <texttree/node.hpp>

namespace tt {

node::node()
{
}

node::node(std::string const& text) : text_(text)
{
}

node::operator bool() const
{
    return false;
}

std::string node::text() const
{
    return text_;
}

} // namespace tt
