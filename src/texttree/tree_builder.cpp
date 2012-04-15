#include <texttree/tree_builder.hpp>
#include <cassert>
#include <fstream>

namespace tt {

tree_builder::tree_builder()
{
    stack_.emplace();
}

node const tree_builder::tree() const
{
    assert(stack_.size() == 1);
    return stack_.top();
}

void tree_builder::begin_node(std::string const& text)
{
    stack_.emplace(text);
}

void tree_builder::end_node()
{
    node child = stack_.top();
    stack_.pop();
    stack_.top().children.push_back(child);
}

node const load(std::string const& string)
{
    tree_builder builder;
    parser parser(builder);
    parser.parse(&*begin(string), &*end(string));
    parser.end_parse();
    return builder.tree();
}

node const load_file(char const* filename)
{
    tree_builder builder;
    parser parser(builder);
    std::ifstream file(filename);

    while (file.good())
    {
        char buffer[1024];
        file.read(buffer, sizeof(buffer));
        parser.parse(buffer, buffer + file.gcount());
    }

    parser.end_parse();
    return builder.tree();
}

} // namespace tt
