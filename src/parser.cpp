#include "parser.hpp"
#include "lex.hpp"
#include "utility.hpp"
#include <cassert>
#include <ranges>
#include <iostream>

Expression make_operation(std::string op, Operation::OperationType type, std::vector<Expression> operands, float precedence)
{
    return std::make_shared<Operation>(op, type, std::move(operands), precedence);
}

Expression make_instruction(std::string opcode, std::vector<Expression> operands)
{
    return std::make_shared<Operation>(opcode, Operation::OperationType::Instruction, std::move(operands), 0);
}

void Operation::print_tree(int indent) const
{
    if (type == OperationType::Instruction)
    {
        std::cout << op << "\n";
        for (const auto &operand : operands)
        {
            std::visit([](const auto &expr)
                       {
                using T = std::decay_t<decltype(expr)>;
                if constexpr (std::is_same_v<T, Atom>) {
                    expr.print_tree(2);
                } else if constexpr (std::is_same_v<T, std::shared_ptr<Operation>>) {
                    expr->print_tree(2);
                } }, operand);
        }
    }
    else
    {
        std::cout << std::string(indent, ' ') << op << "\n";
        for (const auto &operand : operands)
        {
            std::visit([indent](const auto &expr)
                       {
                using T = std::decay_t<decltype(expr)>;
                if constexpr (std::is_same_v<T, Atom>) {
                    expr.print_tree(indent + 2);
                } else if constexpr (std::is_same_v<T, std::shared_ptr<Operation>>) {
                    expr->print_tree(indent + 2);
                } }, operand);
        }
    }
}

void Parser::print_expression_tree(const Expression &expr, int indent) const
{
    std::visit([indent](const auto &e)
               {
        using T = std::decay_t<decltype(e)>;
        if constexpr (std::is_same_v<T, Atom>) {
            e.print_tree(indent);
        } else if constexpr (std::is_same_v<T, std::shared_ptr<Operation>>) {
            e->print_tree(indent);
        } }, expr);
}

void Parser::print_all_trees() const
{
    for (const auto &expr : expressions)
    {
        print_expression_tree(expr);
        std::cout << "-------------------\n";
    }
}

Parser::Parser(Lexer &&lex) : m_lexer{std::move(lex)}, m_lex_tokens{m_lexer.tokens()}
{
    m_lexer.run();
    current_token = m_lex_tokens.begin();
}

Parser::Parser(const char *file_name) : m_lexer{Lexer(file_name)}, m_lex_tokens{m_lexer.tokens()}
{
    m_lexer.run();
    current_token = m_lex_tokens.begin();
}

const Token &Parser::peek()
{
    if (current_token + 1 >= m_lex_tokens.end())
    {
        throw std::out_of_range("Cannot peek beyond the end of tokens");
    }
    return *(current_token + 1);
}

const Token &Parser::peek_back()
{
    if (current_token - 1 < m_lex_tokens.begin())
    {
        throw std::out_of_range("Cannot peek beyond the beginning of tokens");
    }
    return *(current_token - 1);
}

const Token &Parser::next()
{
    if (current_token >= m_lex_tokens.end())
    {
        throw std::out_of_range("Cannot get token beyond the end of tokens");
    }
    return *(current_token++);
}

const Token &Parser::current()
{
    return *current_token;
}

void Parser::skip_comment()
{
    while (current_token != m_lex_tokens.end())
    {
        next();
        if (current().type() == Token::Type::Newline || current().type() == Token::Type::Eof)
        {
            return;
        }
    }
}

std::optional<int> Parser::get_prefix_binding_power(Token::Type type)
{
    switch (type)
    {
    default:
        return std::nullopt;
    }
}

std::pair<int, int> Parser::get_infix_binding_power(Token::Type type)
{
    switch (type)
    {
    case Token::Type::Add:
        return {10, 11};
    case Token::Type::Subtract:
        return {10, 11};
    case Token::Type::Multiply:
        return {20, 21};
    default:
        return {0, 0};
    }
}

Expression Parser::parse_expr()
{
    auto token = next();

    if (token.type() == Token::Type::LeftSquare)
    {
        auto inner = parse_operand();
        if (current().type() != Token::Type::RightSquare)
        {
            throw std::runtime_error("expected closing bracket");
        }
        next();
        return inner;
    }

    return Atom{token.value()};
}

Expression Parser::parse_operand()
{
    auto left = parse_expr();

    while (current_token != m_lex_tokens.end())
    {
        auto &token = current();

        if (token.type() == Token::Type::Separator ||
            token.type() == Token::Type::Newline ||
            token.type() == Token::Type::Eof ||
            token.type() == Token::Type::RightSquare ||
            token.type() == Token::Type::Comment)
        {
            break;
        }

        auto [left_bp, right_bp] = get_infix_binding_power(token.type());

        if (left_bp == 0)
        { // not an operator
            break;
        }

        next();
        auto right = parse_operand();

        left = make_operation(
            token.value(),
            Operation::OperationType::Arithmetic,
            {std::move(left), std::move(right)},
            left_bp);
    }

    return left;
}

Expression Parser::parse_instruction()
{
    std::string opcode = current().value();
    next(); // consume opcode

    std::vector<Expression> operands;

    while (current_token != m_lex_tokens.end())
    {
        if (current().type() == Token::Type::Comment)
        {
            skip_comment();
            break;
        }

        if (current().type() == Token::Type::Newline || current().type() == Token::Type::Eof)
        {
            break;
        }

        if (current().type() == Token::Type::Separator)
        {
            next();
            continue;
        }

        operands.push_back(parse_operand());
    }

    if (current_token != m_lex_tokens.end() &&
        (current().type() == Token::Type::Newline || current().type() == Token::Type::Eof))
    {
        next();
    }

    return make_instruction(opcode, std::move(operands));
}

void Parser::parse()
{
    while (current_token != m_lex_tokens.end())
    {
        if (current().type() == Token::Type::Identifier)
        {
            expressions.push_back(parse_instruction());
        }
        else if (current().type() == Token::Type::Comment)
        {
            skip_comment();
        }
        else
        {
            next();
        }
    }
    print_all_trees();
}
