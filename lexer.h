#ifndef NAIVELEX_LEXER_H
#define NAIVELEX_LEXER_H

#include <string>
#include "token.h"
#include "file.h"
#include <vector>

class naiveLex {
    void skip_line_comment();
    void skip_block_comment();
    Token get_next_numeric_token(TokenType prev_type);
    Token get_next_string_literial_token();
    Token get_next_character_constant_token();
    Token get_next_identifier_token();
    Token get_next_preprocessing_directive_token();
    FileWrapper fileWrapper_;
    TokenLoc currentLocation_;
    std::string tokenBuffer_;
public:
    explicit naiveLex(const std::string& fileName) : fileWrapper_{fileName} {}
    Token getNextToken(TokenType prev_type);
    size_t getCount(){          return fileWrapper_.getCount();        }
    size_t getLineCount(){      return fileWrapper_.getLineCount();    }
    std::string getSrcName(){   return fileWrapper_.getName();         }
};


#endif //NAIVELEX_LEXER_H