#ifndef TEXTTREE_NODE_HPP
#define TEXTTREE_NODE_HPP

#include <string>

namespace tt {

class node
{
public:
    node();
    node(std::string const& text);

    operator bool() const;
    std::string text() const;

private:
    std::string text_;
};

} // namespace tt

#endif // TEXTTREE_NODE_HPP
