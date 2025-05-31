#include "parser.hpp"
#include "lex.hpp"
#include "utility.hpp"

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

const Token& Parser::get()
{
    if (current_token >= m_lex_tokens.end())
    {
        throw std::out_of_range("Cannot get token beyond the end of tokens");
    }
    return *(current_token++);
};


void Parser::parse() {
    _print_iterable(m_lex_tokens);
};