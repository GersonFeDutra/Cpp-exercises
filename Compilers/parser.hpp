#pragma once

#include <istream>
#include <ostream>
#include <format>

//using namespace std::string_literals;

/** Custom Exception: SyntaxError
 * @brief Throws when this compiler encounters a syntax error in the input source.
 *
 * @note For a real project, we don't recommend the use of exceptions
 *       (reason is overhead). It's here just for educational purposes.
 **/
class SyntaxError : public std::exception
{
    std::string message;
public:
    SyntaxError(const char* message = "") :
        message(std::format("[Syntax Error] {}", message)) {}
    SyntaxError(std::string message) :
        message(std::format("[Syntax Error] {}", message)) {}

    virtual const char* what() const noexcept
    {
        return message.c_str();
    }
};

template <bool no_optimize>
class Parser
{
    std::istream& source;
    std::ostream& translation;
    std::string word;
    size_t position = 0;
    char lookahead;
    int accumulator = 0;
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
            throw SyntaxError(std::format("Expected '{}', got '{}' instead.", t, lookahead));
    }

    /* Regra: digit -> digit { print(digit) } */
    inline int digit() {
        int value = 0;
        if (isdigit(lookahead)) {
            value = lookahead - '0';
            while (isdigit(lookahead = get_next_char())) {
                value *= 10;
                value += lookahead - '0';
            }
        }
        else
            throw SyntaxError(std::format("Expected a digit. Got '{}' instead.", lookahead));

        if constexpr(no_optimize)
            translation << value << std::flush;
        return value;
    }

    /** Regra: expr -> digit oper.
     * @note if no_optimize is false, the translator will use the accumulator.
     **/
    inline void expr() {
        accumulator = digit();
        for (;;) {
            switch (lookahead)
            {
            /* Regra: oper -> + digit { print(+) } oper */
            case '+': {
                match('+');
                accumulator += digit();
                if constexpr(no_optimize)
                    translation << '+' << std::flush; // flushing enabled (may be slow)
            } break;

            /* # Regra: oper -> - digit { print(-) } oper */
            case '-': {
                match('-');
                accumulator -= digit();
                if constexpr(no_optimize)
                    translation << '-' << std::flush;
            } break;

            /* Produção vazia (return) */
            default:
                if constexpr(no_optimize)
                    translation << std::endl;
                else
                    translation << accumulator << std::endl;
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
