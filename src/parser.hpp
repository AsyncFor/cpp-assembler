#pragma once
#include "lex.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>

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

class Parser 
{
public:
    Parser(Lexer &&lex);
    Parser(const char *file_name);
    void parse();
    bool has_errors() const;
    const std::vector<std::string>& get_errors() const;

    std::vector<Token>::const_iterator current_token;
    std::vector<Expression> expressions;
    
    const Token& peek();
    const Token& get();

private:
    Lexer m_lexer;
    const std::vector<Token> &m_lex_tokens;
    std::vector<Expression> m_ast;

};
