#include "../utils/parser.hpp"
#include <fstream>
#include <iostream>
#include <string>


#include <iostream>
#include <string>

ParsedInt parse_int(const std::string &line, size_t from);
ParsedInt parse_int(const std::string &line, size_t from)
{
	ParsedSubstring parse = parse_string(line, from);

	return {
		std::stoi(parse.value, nullptr, 10),
		parse.at,
	};
}


struct Graph {
	std::string name;
	size_t n_vertexes;
	size_t m_edges;

	Graph() : name(""), n_vertexes(0), m_edges(0)
	{
	}

	operator bool()
	{
		return !name.empty();
	}
};

Graph read_graph()
{
	Graph graph;
	std::string out;
	std::ifstream file("graph.txt");

	if (file.is_open()) {
		getline(file, out);

		auto parsed_s = parse_string(out, 0);
		graph.name = parsed_s.value;

		auto parsed_i = parse_int(out, parsed_s.at + 1);
		graph.n_vertexes = parsed_i.value;

		parsed_i = parse_int(out, parsed_i.at + 1);
		graph.m_edges = parsed_i.value;
	}
	else
		std::cerr << "\33[31m"
					 "Unable to close file"
					 "\33[m"
				  << '\n';

	return graph;
}


int main(void)
{
	if (Graph graph = read_graph()) {
		std::cout << "Graph " << graph.name << '\n';
		std::cout << "n: " << graph.n_vertexes << ", m: " << graph.m_edges << '\n';
	}
}
