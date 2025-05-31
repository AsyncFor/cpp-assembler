#include "lex.hpp"
#include <vector>
#include <iostream>
#include <variant>
#include "utility.hpp"


struct Atom
{
    std::string value;
    explicit Atom(std::string v) : value(v) {}
};

struct Operation;

using Expression = std::variant<Atom, std::shared_ptr<Operation>>;

struct Operation
{

    enum struct OperationType
    {
        Instruction,
        Arithmetic
    };

    OperationType type;
    int binding_power;
    std::string op;
    std::vector<Expression> operands;

    Operation(std::string o, OperationType op_type, std::vector<Expression> ops, int b_power) : op{o}, operands{std::move(ops)}, type{op_type}, binding_power{b_power} {}
};

Expression make_atom(std::string value)
{
    return Atom(value);
}

Expression make_operation(std::string operation, Operation::OperationType op_type, std::vector<Expression> operands, int binding_power = 0)
{
    return std::make_shared<Operation>(operation, op_type, std::move(operands), binding_power);
}

class Parser
{
public:
    Parser(Lexer &&lex) : m_lexer{std::move(lex)}, m_lex_tokens{m_lexer.tokens()}
    {
        m_lexer.run();
        current_token = m_lex_tokens.begin();
    };
    Parser(const char *file_name) : m_lexer{Lexer(file_name)}, m_lex_tokens{m_lexer.tokens()}
    {
        m_lexer.run();
        current_token = m_lex_tokens.begin();
    };

    std::vector<Token>::const_iterator current_token;
    std::vector<Expression> expressions;

    const Token &peek()
    {
        if (current_token + 1 >= m_lex_tokens.end())
        {
            throw std::out_of_range("Cannot peek beyond the end of tokens");
        }
        return *(current_token + 1);
    };

    const Token &get()
    {
        if (current_token >= m_lex_tokens.end())
        {
            throw std::out_of_range("Cannot get token beyond the end of tokens");
        }
        return *(current_token++);
    };

    void parse() {
         _print_iterable(m_lex_tokens);
    };

private:
    Lexer m_lexer;
    const std::vector<Token> &m_lex_tokens;
    std::vector<Expression> m_ast;
};

int main()
{

    std::cout << "Starting assembler..." << std::endl;
    Lexer lex("file.asm");
    lex.run();

    Parser parser(std::move(lex));
    parser.parse();

}