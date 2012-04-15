TextTree
========

TextTree is a grammar for describing very simple hierarchical text data and a C++ library that can load (but not yet save) such data.

Examples
--------

*Dictionary*

    object(
      key1(value1)
      key2(value2)
      key3(value3))

Here `object` is a node with three children. The text of each child acts as the key and the child of the key contains the value.

Parenthesis are omitted be skipped after each value because they are empty anyway. The strings are written in unquoted style becase there are no spaces. Without these tricks would make the example look like this instead.

    "object"(
      "key1"("value1"())
      "key2"("value2"())
      "key3"("value3"()))

*Lists*

    list(value1 value2 value3)

In this example `list` contains three child nodes. If we want to have a list of strings that may contain spaces just write like this instead:

    list("value 1" "value 2" "value 3")

Grammar
-------

    elements = { node }
    node     = text [ list ]
    text     = unquoted | quoted
    list     = '(' elements ')'
    unquoted = ? any characters except space or parenthesis ?
    quoted   = '"' ? any characters with \" or \\ as escapes ? '"'

Library
-------

The library uses C++11 and intends to use modern C++ idioms.

The library contains a `parser` that generates events to a `parser_delegate`. The `parser_delegate` may be subclassed to create custom data structures or do streaming parsing. There is also one `parser_delegate` implementation called `tree_builder` that builds a hiearchy of `node` objects. `tree_builder` also comes with a few helper functions to make loading files really simple:

*Example*

    #include <texttree/tree_builder.hpp>
    
    tt::node_ptr node = tt::load_file("myfile.txt");

Building with CMake
-------------------

If you use CMake and want to include the library in your code you can put the source code in your project tree (see [Working with submodules](http://help.github.com/submodules/) to learn how to do it with Git) and write something like the following at proper places in your CMakeLists.txt

    set(TEXTTREE_ROOT ...)                           # Path to TextTree root directory
    add_subdirectory(${TEXTTREE_ROOT}/src)           # Run TextTree's CMake file
    include_directories(${TEXTTREE_ROOT}/src)        # Path to headers
    target_link_libraries(your_application texttree) # Link with texttree library

If you want to run the unit tests then you should build the root CMakeFile.txt instead. TextTree uses the Google testing framework which you must build separately. Then pass -DCMAKE_INCLUDE_PATH=*path to gtest headers* and -DCMAKE_LIBRARY_PATH=*path to gtest libraries* to cmake when generating the makefiles.