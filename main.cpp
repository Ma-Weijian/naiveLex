#include <iostream>
#include <fstream>
#include "file.h"
#include "token.h"
#include "lexer.h"
#include <cstdlib>
#include <string>

void init(int argc, char **argv) {
    std::cout<<"              _           _               "<<std::endl;
    std::cout<<"             (_)         | |              "<<std::endl;
    std::cout<<"  _ __   __ _ ___   _____| |     _____  __"<<std::endl;
    std::cout<<" | '_ \\ / _` | \\ \\ / / _ \\ |    / _ \\ \\/ /"<<std::endl;
    std::cout<<" | | | | (_| | |\\ V /  __/ |___|  __/>  < "<<std::endl;
    std::cout<<" |_| |_|\\__,_|_| \\_/ \\___|______\\___/_/\\_\\"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Welcome to naiveLex."<<std::endl;

    if(argc < 2 || argc > 3)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "naiveLex <input file name> [output file name]" << std::endl;
        exit(0);
    }
}

int main(int argc, char** argv) {
    init(argc, argv);

    auto counter = Counter();
    auto lex_op = naiveLex(argv[1]);
    std::ofstream out(argv[2]);
    if(argc == 3)
    {
        std::cout<<"The output file has been redirected to "<<argv[2]<<std::endl;
        std::cout.rdbuf(out.rdbuf());
    }
    std::cout << "Tokens:" << std::endl;
    TokenType prev_type = (TokenType)0;
    Token tok = lex_op.getNextToken(prev_type);
    while(tok.getTokenType() != TokenType::EndOfFile) {
        std::cout << tok << std::endl;
        counter.update(tok);
        prev_type = tok.getTokenType();
        tok = lex_op.getNextToken(prev_type);
    }
    std::cout << "File info:" << std::endl;
    std::cout << "File name:\t" << lex_op.getSrcName() << std::endl;
    std::cout << "Total chars:\t" << lex_op.getCount() << std::endl;
    std::cout << "Total lines:\t" << lex_op.getLineCount() << std::endl;
    std::cout << counter;
    return 0;
}