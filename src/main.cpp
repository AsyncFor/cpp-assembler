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
            Add,
            Multiply,
            Subtract,
            Separator,
            Identifier,
            LeftSquare,
            RightSquare,
            Label
        };
        

    Token(Type type) noexcept : m_type{type} {};
    Token(Type type, std::string value) noexcept: m_type{type}, m_value{value} {};
    private:
        Type m_type{};
        std::string m_value{};

};

bool is_numerical(const std::string& string) {
    std::string::const_iterator itr = string.begin();
    while (itr != string.end() && std::isdigit(*itr)) ++itr;
    return !string.empty() && itr == string.end();
}


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
                Token token(Token::Type::Add, current_word);
                tokens.push_back(token);
                current_word.clear();
                continue;
            }
            case '-':
            {
                current_word.push_back(current);
                Token token(Token::Type::Subtract, current_word);
                tokens.push_back(token);
                current_word.clear();
                continue;
            }
            case '*':
            {
                current_word.push_back(current);
                Token token(Token::Type::Multiply, current_word);
                tokens.push_back(token);
                current_word.clear();
                continue;
            }
            case '[':
            {
                current_word.push_back(current);
                Token token(Token::Type::LeftSquare, current_word);
                tokens.push_back(token);
                current_word.clear();
                continue;
            }
            case ']':
            {
                current_word.push_back(current);
                Token token(Token::Type::RightSquare, current_word);
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

        char peek = input_file.peek();
        // if peek is a separator add current word as identifier
        if (peek == ' ' || peek == '\n' || peek == ',' || peek == '[' || peek == ']' || peek == '+' || peek == '-' || peek == '*' || input_file.eof()) {
            current_word.push_back(current);
            

            // if current word is numerical we use number type
            if (is_numerical(current_word)) {
                Token token(Token::Type::Number, current_word);
                tokens.push_back(token);
            } else {
                Token token(Token::Type::Identifier, current_word);
                tokens.push_back(token);
            }


            current_word.clear();
            
        } else {
            current_word.push_back(current);
        }

        if (input_file.eof()) {
            break;
        }

    }


}