#include "lex.hpp"
#include "parser.hpp"
#include <vector>
#include <iostream>
#include <variant>
#include "utility.hpp"




int main()
{

    std::cout << "Starting assembler..." << std::endl;
    Lexer lex("file.asm");
    lex.run();

    Parser parser(std::move(lex));
    parser.parse();

}