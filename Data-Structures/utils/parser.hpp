#pragma once
#include <string>

struct ParsedSubstring {
	std::string value;
	size_t at;
};
struct ParsedInt {
	int value;
	size_t at;
};

ParsedSubstring parse_string(const std::string &line, size_t from);
