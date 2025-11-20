#include <iostream>
#include <fstream>

#include "parser.hpp"

void print_error(const char *message);

/** Postfix : Equivalente ao main() do postfix.cpp
 * @brief Compiles a simple language source file to postfix.
 *
 * @note Usage: ./postfix <source-file-name> ... [<output-file-name>]
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
        if (!output_file.is_open())
            print_error("[File Error]: Source File not found");
            return EXIT_FAILURE;
    }
    if (!source.is_open()) {
        print_error("[File Error]: Output File not found");
        return EXIT_FAILURE;
    }

    Parser parser(source, *translation);
    try {
        parser.start();
    }
    catch (SyntaxError& e) {
        print_error(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


void print_error(const char *message)
{
    std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
}
