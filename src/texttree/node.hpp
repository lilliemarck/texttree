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
    typedef std::vector<node_ptr>::iterator iterator;
    typedef std::vector<node_ptr>::const_iterator const_iterator;

    node();
    node(std::string const& text);

    std::string const text() const;
    void set_text(std::string const& text);

    void append_child(node_ptr const& child);
    size_t const child_count() const;
    node_ptr const child_at(size_t index) const;

    iterator const begin();
    const_iterator const begin() const;
    iterator const end();
    const_iterator const end() const;

private:
    std::string text_;
    std::vector<node_ptr> children_;
};

node_ptr const find(node const& node, std::string const& text);

} // namespace tt

#endif // TEXTTREE_NODE_HPP
