#include "graph.hpp"
#include <set>


std::ifstream& operator>>(std::ifstream& stream, Graph& g)
{
    std::set<std::string> nodes;
    //g.add()

    while (stream) {
        std::string n1, n2;
        //int weight; // implement
        stream >> n1 >> n2;
        //g.add({nullptr, }, {nullptr, });
    }
}
