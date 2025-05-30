#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
void _print_iterable(T iterable) {
    std::cout << "Iterable " << "{\n";
    for (const auto& data : iterable) {
        std::cout << "\t" << data << ",\n";
    }
    std::cout << "}" << std::endl;
};


class Token {
    public:
        enum class Type {
            Number,
            Add,
            Multiply,
            Subtract,
            Separator,
            Identifier,
            LeftSquare,
            RightSquare,
            Label
        };
        
    friend std::ostream& operator<<(std::ostream& os, const Type& type) {
        switch (type) {
            case Type::Number: os << "Number"; break;
            case Type::Add: os << "Add"; break;
            case Type::Multiply: os << "Multiply"; break;
            case Type::Subtract: os << "Subtract"; break;
            case Type::Separator: os << "Separator"; break;
            case Type::Identifier: os << "Identifier"; break;
            case Type::LeftSquare: os << "LeftSquare"; break;
            case Type::RightSquare: os << "RightSquare"; break;
            case Type::Label: os << "Label"; break;
        }
        return os;
    }

    Token(Type type) noexcept : m_type{type} {};
    Token(Type type, std::string value) noexcept: m_type{type}, m_value{value} {};

    
    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token<" << token.m_type << ">" << "('" << token.m_value << "')";
        return os;      
    }

    private:
        Type m_type{};
        std::string m_value{};

};


class Lexer {
    public:

    Lexer(const char* filename) : m_input_file{std::ifstream(filename)} {
        if (!m_input_file.is_open()) {
            throw std::runtime_error("failed to open file");
        }
    };

    void run() {

        std::string current_word = "";
        
        while (char current = m_input_file.get()) {



            switch (current) {
                case ' ': continue;
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
                    Token token(Token::Type::Label, current_word);
                    m_tokens.push_back(token);
                    current_word.clear();
                }
                case '\n': 
                    continue;
            }

            char peek = m_input_file.peek();
            // if peek is a separator add current word as identifier
            if (peek == ' ' || peek == '\n' || peek == ',' || peek == '[' || peek == ']' || peek == '+' || peek == '-' || peek == '*' || m_input_file.eof()) {
                current_word.push_back(current);
                

                // if current word is numerical we use number type
                if (is_numerical(current_word)) {
                    Token token(Token::Type::Number, current_word);
                    m_tokens.push_back(token);
                } else {
                    Token token(Token::Type::Identifier, current_word);
                    m_tokens.push_back(token);
                }


                current_word.clear();
                
            } else {
                current_word.push_back(current);
            }

            if (m_input_file.eof()) {
                break;
            }

        }
    }

    const std::vector<Token>& tokens() {
        return m_tokens;
    }
    
    
    bool is_numerical(const std::string& string) {
        std::string::const_iterator itr = string.begin();
        while (itr != string.end() && std::isdigit(*itr)) ++itr;
        return !string.empty() && itr == string.end();
    }

    private:
        std::vector<Token> m_tokens{};
        std::ifstream m_input_file{};
};



int main() {
    
    std::cout << "Starting assembler..." << std::endl;
    Lexer lex("file.asm");
    lex.run();
    _print_iterable(lex.tokens());






}