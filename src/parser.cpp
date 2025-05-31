#include "parser.hpp"
#include "lex.hpp"
#include "utility.hpp"
#include <cassert>




Expression make_operation(std::string op, Operation::OperationType type, std::vector<Expression> operands, int precedence) {
    return std::make_shared<Operation>(op, type, std::move(operands), precedence);
}

Parser::Parser(Lexer &&lex) : m_lexer{std::move(lex)}, m_lex_tokens{m_lexer.tokens()}
{
    m_lexer.run();
    current_token = m_lex_tokens.begin();
};

Parser::Parser(const char *file_name) : m_lexer{Lexer(file_name)}, m_lex_tokens{m_lexer.tokens()}
{
    m_lexer.run();
    current_token = m_lex_tokens.begin();
};

const Token& Parser::peek()
{
    if (current_token + 1 >= m_lex_tokens.end())
    {
        throw std::out_of_range("Cannot peek beyond the end of tokens");
    }
    return *(current_token + 1);
};


const Token& Parser::peek_back()
{
    if (current_token - 1 < m_lex_tokens.begin())
    {
        throw std::out_of_range("Cannot peek beyond the beginning of tokens");
    }
    return *(current_token - 1);
};

const Token& Parser::get()
{
    if (current_token >= m_lex_tokens.end())
    {
        throw std::out_of_range("Cannot get token beyond the end of tokens");
    }
    return *(current_token++);
};


bool is_operator(const Token::Type token_type) {
    switch (token_type) {
        case Token::Type::Add:
            return 1;
        case Token::Type::Subtract:
            return 1;
        case Token::Type::Multiply:
            return 1;
    }
    return 0;
}
float get_operator_precedence(const Token::Type op) {
    switch (op) {
        case Token::Type::Add:
            return 10;
        case Token::Type::Subtract:
            return 9;
        case Token::Type::Multiply:
            return 100;
        default:
            return 0;    
    }
}
