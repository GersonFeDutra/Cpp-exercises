#include <iostream>
#include "graph.hpp"

#define RED(S) "\33[33m;" ## S ## "\33[m;"
#define print_err(W) std::cerr << "Error: " << RED(W) << '\n'

void tui(Graph&, std::string&);


struct GraphReader
{
    std::ifstream file;
    std::size_t m = 0, n = 0;
    std::string label;
};

inline GraphReader start(const char* name)
{
    GraphReader data {
        .file=std::ifstream{"name"},
    };

    if (data.file.is_open()) {
        data.file >> data.label >> data.m >> data.n;
    }
    else {
        print_err("File not opened");
    }

    return data;
}


int main(int argc, char const *argv[])
{
    if (argc) {
        GraphReader reader = start(argv[1]);

        if (argc == 2 && argv[2][0] == '-' && argv[2][1] == 's') {
            // TODO -> make runtime version of static graph
            //StaticGraph<reader.m, reader.n> g;
            //reader.file >> g;
            //std::cout << g;
            //tui(g, reader.label);
        }
        else {
            DynamicGraph g{reader.m, reader.n};

            reader.file >> g;
            std::cout << g;

            tui(g, reader.label);
        }

        std::ifstream file();
    }
}


inline void tui(Graph &g, std::string& name) // Terminal User Interface
{
    /* code */
}
