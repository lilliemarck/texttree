#include <texttree/parser.hpp>
#include <texttree/node.hpp>
#include <algorithm>
#include <cctype>
#include <string>

namespace tt {

namespace
{
    node_ptr parse_unquoted_string(std::string::const_iterator begin,
                                   std::string::const_iterator end)
    {
        auto end_of_string = std::find_if(begin, end, [](char c) {
            return isspace(c);
        });

        return std::make_shared<node>(std::string(begin, end_of_string));
    }
} // namespace

node_ptr parse(const std::string& string)
{
    for (auto iterator = begin(string); iterator != end(string); ++iterator)
    {
        if (!isspace(*iterator))
        {
            return parse_unquoted_string(iterator, end(string));
        }
    }

    return node_ptr();
}

} // namespace tt
