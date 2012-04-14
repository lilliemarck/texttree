#ifndef TEXTTREE_NODE_HPP
#define TEXTTREE_NODE_HPP

#include <memory>
#include <string>
#include <vector>

namespace tt {

typedef std::shared_ptr<class node> node_ptr;

class node
{
public:
    node();
    node(std::string const& text);

    std::string const text() const;

    void append_child(node_ptr const& child);
    size_t const child_count() const;
    node_ptr const child_at(size_t index) const;

private:
    std::string text_;
    std::vector<node_ptr> children_;

};

} // namespace tt

#endif // TEXTTREE_NODE_HPP
