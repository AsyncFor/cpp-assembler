#include "lex.hpp"

Token::Token(Type type) noexcept : m_type{type} {}
Token::Token(Type type, std::string value) noexcept : m_type{type}, m_value{value} {}
Token::Type Token::type() const { return m_type; }

const std::string &Token::value() const { return m_value; }

Token::operator bool() const
{
    return m_type != Type::Eof;
}

std::ostream &operator<<(std::ostream &os, const Token::Type &type)
{
    switch (type)
    {
    case Token::Type::Number:
        os << "Number";
        break;
    case Token::Type::Add:
        os << "Add";
        break;
    case Token::Type::Multiply:
        os << "Multiply";
        break;
    case Token::Type::Subtract:
        os << "Subtract";
        break;
    case Token::Type::Separator:
        os << "Separator";
        break;
    case Token::Type::Identifier:
        os << "Identifier";
        break;
    case Token::Type::LeftSquare:
        os << "LeftSquare";
        break;
    case Token::Type::RightSquare:
        os << "RightSquare";
        break;
    case Token::Type::Label:
        os << "Label";
        break;
    case Token::Type::Eof:
        os << "Eof";
        break;
    case Token::Type::Newline:
        os << "New Line";
        break;
    case Token::Type::Comment:
        os << "Comment";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Token &token){
    {os << "Token<" << token.m_type << ">" << "('" << token.m_value << "')";
return os;
}
}
;

Lexer::Lexer(const char *filename) : m_input_file{std::ifstream(filename)}
{
    if (!m_input_file.is_open())
    {
        throw std::runtime_error("failed to open file");
    }
};

void Lexer::run()
{

    if (has_run)
        return;

    std::string current_word = "";

    while (char current = m_input_file.get())
    {

        switch (current)
        {
        case ' ':
        {
            continue;
        }
        case ',':
        {
            current_word.push_back(current);
            Token token(Token::Type::Separator, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case '+':
        {
            current_word.push_back(current);
            Token token(Token::Type::Add, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case '-':
        {
            current_word.push_back(current);
            Token token(Token::Type::Subtract, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case '*':
        {
            current_word.push_back(current);
            Token token(Token::Type::Multiply, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case '[':
        {
            current_word.push_back(current);
            Token token(Token::Type::LeftSquare, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case ']':
        {
            current_word.push_back(current);
            Token token(Token::Type::RightSquare, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case ':':
        {
            current_word.push_back(current);
            Token token(Token::Type::Label, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case ';':
        {
            current_word.push_back(current);
            Token token(Token::Type::Comment, current_word);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        case '\n':
        {
            current_word.push_back(current);
            Token token(Token::Type::Newline);
            m_tokens.push_back(token);
            current_word.clear();
            continue;
        }
        }

        char peek = m_input_file.peek();
        // if peek is a separator add current word as identifier
        if (peek == ' ' || peek == '\n' || peek == ',' || peek == '[' || peek == ']' || peek == '+' || peek == '-' || peek == '*' || peek == ';' || m_input_file.eof())
        {
            current_word.push_back(current);

            // if current word is numerical we use number type
            if (is_numerical(current_word))
            {
                Token token(Token::Type::Number, current_word);
                m_tokens.push_back(token);
            }
            else
            {
                Token token(Token::Type::Identifier, current_word);
                m_tokens.push_back(token);
            }

            current_word.clear();
        }
        else
        {
            current_word.push_back(current);
        }

        if (m_input_file.eof())
        {
            break;
        }
    }

    // add eof to tokens
    m_tokens.push_back(Token(Token::Type::Eof));

    has_run = true;
}

const std::vector<Token> &Lexer::tokens()
{
    return m_tokens;
}

bool Lexer::is_numerical(const std::string &string)
{
    {
        std::string::const_iterator itr = string.begin();
        while (itr != string.end() && std::isdigit(*itr))
            ++itr;
        return !string.empty() && itr == string.end();
    }
}
