#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
void _print_iterable(T iterable) {
    std::cout << "Iterable" << "{\n";
    for (const auto& data : iterable) {
        std::cout << data << ",\n";
    }
    std::cout << "}" << std::endl;
};


class Token {
    public:
        enum class Type {
            Number,
            Operator,
            Separator,
            Identifier,
            Label
        };
        

    Token(Type type) noexcept : m_type{type} {};
    Token(Type type, std::string value) noexcept: m_type{type}, m_value{value} {};
    private:
        Type m_type{};
        std::string m_value{};

};

int main() {
    
    std::cout << "Starting assembler..." << std::endl;
    std::ifstream input_file("file.asm");
    if (!input_file.is_open()) {
        std::cout << "failed to open file";
        return 1;
    }
    std::string current_word = "";

    std::vector<Token> tokens{};

    while (char current = input_file.get()) {



        switch (current) {
            case ' ': continue;
            case ',':
            {
                current_word.push_back(current);
                Token token(Token::Type::Separator, current_word);
                tokens.push_back(token);
                current_word.clear();
                continue;
            }
            case '+':
            {
                current_word.push_back(current);
                Token token(Token::Type::Operator, current_word);
                tokens.push_back(token);
                current_word.clear();
                continue;
            }
            case ':':
            {
                Token token(Token::Type::Label, current_word);
                tokens.push_back(token);
                current_word.clear();
            }
            case '\n': 
                continue;
        }

        if (input_file.peek() == ' ' || input_file.peek() == '\0' || input_file.peek() == '\n' || input_file.peek() == ',' || input_file.eof()) {
            current_word.push_back(current);
            
            Token token(Token::Type::Identifier, current_word);
            tokens.push_back(token);
            current_word.clear();
            
        } else {
            current_word.push_back(current);
        }

        if (input_file.eof()) {
            break;
        }

    }


}