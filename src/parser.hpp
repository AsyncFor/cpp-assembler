#pragma once
#include "lex.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>
#include <array>
#include <optional>
#include <utility>


struct Atom
{
    enum class Type { Regular, Label };
    
    std::string value;
    Type type;
    
    explicit Atom(std::string v, Type t = Type::Regular) : value(v), type(t) {}
    void print_tree(int indent = 0) const {
        std::cout << std::string(indent, ' ') << value << "\n";
    }
};


inline Atom make_label(const std::string& name) {
    return Atom{name, Atom::Type::Label};
}



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
    void print_tree(int indent = 0) const;
};



class Instruction {
    public:
        std::string opcode;
        Expression op_1;
        Expression op_2;
        Expression op_3;
        
        

        Instruction(std::string _opcode) : opcode{_opcode}, op_1{Atom("")}, op_2{Atom("")}, op_3{Atom("")} {};
        Instruction(std::string _opcode, Expression op_a) : opcode{_opcode}, op_1{op_a}, op_2{Atom("")}, op_3{Atom("")} {};
        Instruction(std::string _opcode, Expression op_a, Expression op_b) : opcode{_opcode}, op_1{op_a}, op_2{op_b}, op_3{Atom("")} {};
        Instruction(std::string _opcode, Expression op_a, Expression op_b, Expression op_c) : opcode{_opcode}, op_1{op_a}, op_2{op_b}, op_3{op_c}{};    
};



class Parser 
{
public:
    Parser(Lexer &&lex);
    Parser(const char *file_name);
    void parse();
    Expression parse_expr();
    Expression parse_operand();
    Expression parse_instruction();
    void skip_comment();
    std::optional<int> get_prefix_binding_power(Token::Type type);
    std::pair<int, int> get_infix_binding_power(Token::Type type);
    const Token& peek();
    const Token& peek_back();
    const Token& next();
    const Token& current();
    void print_expression_tree(const Expression& expr, int indent = 0) const;
    void print_all_trees() const;

    std::vector<Token>::const_iterator current_token;
    std::vector<Expression> expressions;
    
    
private:
    Lexer m_lexer;
    const std::vector<Token> &m_lex_tokens;
    std::vector<Expression> m_ast;

};
