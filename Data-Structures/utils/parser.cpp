#include "parser.hpp"


ParsedSubstring parse_string(const std::string &line, size_t from)
{
	std::string delimiter = " ";
	size_t at = line.find(delimiter);

	return {
		line.substr(from, at),
		at,
	};
}
