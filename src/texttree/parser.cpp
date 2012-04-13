#include <texttree/parser.hpp>
#include <texttree/node.hpp>
#include <string>

namespace tt {

node_ptr parse(const std::string& string)
{
    if (!string.empty())
    {
        return std::make_shared<node>(string);
    }
    else
    {
        return {};
    }
}

} // namespace tt
