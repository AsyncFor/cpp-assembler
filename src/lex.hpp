#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Token
{
public:
    enum class Type
    {
        Number,
        Add,
        Multiply,
        Subtract,
        Separator,
        Identifier,
        LeftSquare,
        RightSquare,
        Label,
        Eof,
        Newline,
        Comment
    };

    Token(Type type) noexcept;
    Token(Type type, std::string value) noexcept;

    Type type() const;
    const std::string &value() const;

    friend std::ostream &operator<<(std::ostream &os, const Type &type);
    friend std::ostream &operator<<(std::ostream &os, const Token &token);

private:
    Type m_type{};
    std::string m_value{};
};

class Lexer
{
public:
    Lexer(const char *filename);

    void run();
    const std::vector<Token> &tokens();
    bool is_numerical(const std::string &string);

private:
    std::vector<Token> m_tokens{};
    std::ifstream m_input_file{};
    bool has_run = false;
};