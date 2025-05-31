#include <iostream>
#include <fstream>
#include <vector>
#include <variant>

template <typename T>
void _print_iterable(T iterable)
{
    std::cout << "Iterable " << "{\n";
    for (const auto &data : iterable)
    {
        std::cout << "\t" << data << ",\n";
    }
    std::cout << "}" << std::endl;
};

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

    friend std::ostream &operator<<(std::ostream &os, const Type &type)
    {
        switch (type)
        {
        case Type::Number:
            os << "Number";
            break;
        case Type::Add:
            os << "Add";
            break;
        case Type::Multiply:
            os << "Multiply";
            break;
        case Type::Subtract:
            os << "Subtract";
            break;
        case Type::Separator:
            os << "Separator";
            break;
        case Type::Identifier:
            os << "Identifier";
            break;
        case Type::LeftSquare:
            os << "LeftSquare";
            break;
        case Type::RightSquare:
            os << "RightSquare";
            break;
        case Type::Label:
            os << "Label";
            break;
        case Type::Eof:
            os << "Eof";
            break;
        case Type::Newline:
            os << "New Line";
            break;
        case Type::Comment:
            os << "Comment";
            break;
        }
        return os;
    }

    Token(Type type) noexcept : m_type{type} {};
    Token(Type type, std::string value) noexcept : m_type{type}, m_value{value} {};

    friend std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        os << "Token<" << token.m_type << ">" << "('" << token.m_value << "')";
        return os;
    }

    Type type() const
    {
        return m_type;
    }

    const std::string &value() const
    {
        return m_value;
    }

private:
    Type m_type{};
    std::string m_value{};
};

class Lexer
{
public:
    Lexer(const char *filename) : m_input_file{std::ifstream(filename)}
    {
        if (!m_input_file.is_open())
        {
            throw std::runtime_error("failed to open file");
        }
    };

    void run()
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

    const std::vector<Token> &tokens()
    {
        return m_tokens;
    }

    bool is_numerical(const std::string &string)
    {
        std::string::const_iterator itr = string.begin();
        while (itr != string.end() && std::isdigit(*itr))
            ++itr;
        return !string.empty() && itr == string.end();
    }

private:
    std::vector<Token> m_tokens{};
    std::ifstream m_input_file{};
    bool has_run = false;
};

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