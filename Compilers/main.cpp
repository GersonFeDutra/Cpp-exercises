#include <iostream>
#include <fstream>
#include <print>
#include <cstring>

#include "parser.hpp"

inline void print_error(const char *message)
{
    std::print(std::cerr, "\033[31m{}\033[30m\n", message);
}

template <bool no_optimize>
inline void run_parser(Parser<no_optimize> parser) noexcept
try {
    parser.start();
}
catch (SyntaxError& e) {
    print_error(e.what());
    exit(EXIT_FAILURE);
}

/** Postfix : Equivalente ao main() do postfix.cpp
 * @brief Compiles a simple language source file to postfix.
 *
 * @note Usage: ./postfix <source-file-name> ... [-no|--no-optimize] [<output-file-name>]
 *       Only the first argument is source file name is used, currently.
 *       The last argument is the output file path.
 **/
int main(int argc, char* argv[])
{
    // 1. Verifica se o usuário passou o nome do arquivo
    if (argc < 2) {
        print_error("[CLI Error]: Usage: ./postfix <source-file-name>");
        return EXIT_FAILURE;
    }
    std::fstream source(argv[1], std::ios::in);
    std::ostream* translation = &std::cout;
    std::fstream output_file;
    if (argc > 2) {
        output_file.open(argv[argc - 1], std::ios::out);
        translation = &output_file;
        if (!output_file.is_open()) {
            print_error("[File Error]: Source File not found");
            return EXIT_FAILURE;
        }
    }
    if (!source.is_open()) {
        print_error("[File Error]: Output File not found");
        return EXIT_FAILURE;
    }

    bool no_optimize =  argc > 3 &&
        (  std::strcmp(argv[2], "--no-optimize")
        || std::strcmp(argv[2], "-no") == 0);

    if (no_optimize) {
        Parser<true> dumb_parser(source, *translation);
        run_parser(dumb_parser);
    }
    else {
        Parser<false> optimized_parser(source, *translation);
        run_parser(optimized_parser);
    }

    return EXIT_SUCCESS;
}
