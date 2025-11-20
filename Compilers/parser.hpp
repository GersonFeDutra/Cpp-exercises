#pragma once

#include <istream>
#include <ostream>

//using namespace std::string_literals;

/** Custom Exception: SyntaxError
 * @brief Throws when this compiler encounters a syntax error in the input source.
 **/
class SyntaxError : public std::exception
{
public:
    virtual const char* what() noexcept
    {
        return "Syntax Error";
    }
};


class Parser
{
    std::istream& source;
    std::ostream& translation;
    std::string word;
    size_t position = 0;
    char lookahead;
public:
    Parser(std::istream& source, std::ostream& translation) : source(source), translation(translation) {}
    ~Parser() {
        word.clear();
    }

    inline char get_next_char() noexcept {
        // Simula o cin.get() lendo da string armazenada
        if (char c = word[position++]; c != '\0')
            //if (!isspace(c)) // stream read skip spaces
            return c;

        position = 0;
        // Se for espaço, tabulação ou quebra de linha (exceto a última), ignora.
        if (source >> word)
            return word[position++];

        return '\0'; // Fim da entrada
    }

    /* Verifica se o caractere atual corresponde ao esperado e avança. */
    inline void match(char t) {
        if (t == lookahead)
            lookahead = get_next_char();
        else
            throw SyntaxError();
    }

    /* Regra: digit -> digit { print(digit) } */
    inline void digit() {
        if (isdigit(lookahead)) {
            translation << lookahead << std::flush;
            match(lookahead);
        }
        else
            throw SyntaxError();
    }

    /* Regra: expr -> digit oper */
    inline void expr() {
        digit();
        for (;;) {
            switch (lookahead)
            {
            /* Regra: oper -> + digit { print(+) } oper */
            case '+': {
                match('+');
                digit();
                translation << '+' << std::flush; // flushing enabled (may be slow)
            } break;

            /* # Regra: oper -> - digit { print(-) } oper */
            case '-': {
                match('-');
                digit();
                translation << '-' << std::flush;
            } break;

            /* Produção vazia (return) */
            default:
                translation << '\n' << std::flush;
                return;
            }
        }
    }

    /* Inicia o processo de análise lendo o primeiro caractere. */
    void start() {
        // Lê 1 caractere da entrada padrão (equivalente ao cin.get())
        lookahead = get_next_char();
        expr();

        // Verifica se o último caractere é uma quebra de linha
        if (lookahead != '\0')
            throw SyntaxError();
    }
};
