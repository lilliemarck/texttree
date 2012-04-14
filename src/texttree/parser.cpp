#include <texttree/parser.hpp>
#include <texttree/node.hpp>
#include <algorithm>
#include <cctype>
#include <string>

namespace tt {

node_ptr const parse_children(std::string const& string)
{
    tt::node_ptr root = std::make_shared<node>();

    for (auto iterator = begin(string); iterator != end(string);)
    {
        if (*iterator == '\"')
        {
            ++iterator;
            auto end_of_string = std::find_if(iterator, end(string), [](char c) {
                return c == '\"';
            });

            root->append_child(std::make_shared<node>(std::string(iterator, end_of_string)));
            iterator = end_of_string;
        }
        else if (!isspace(*iterator))
        {
            auto end_of_string = std::find_if(iterator, end(string), [](char c) {
                return isspace(c);
            });

            root->append_child(std::make_shared<node>(std::string(iterator, end_of_string)));
            iterator = end_of_string;
            continue;
        }

        ++iterator;
    }

    return root;
}

node_ptr const parse(std::string const& string)
{
    node_ptr node = parse_children(string);
    return node->child_count() > 0 ? node->child_at(0) : node_ptr();
}

} // namespace tt
