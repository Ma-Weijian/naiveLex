#ifndef NAIVELEX_TOKEN_H
#define NAIVELEX_TOKEN_H

#include <string>
#include <iostream>
#include <utility>
#include <cassert>

// ISO C N2176 Section 6.4
enum class TokenType {
    Keyword, Identifier, NumericConstant, NumericConstantWithError, CharacterConstant,
    StringLiteral ,Punctuator, PreprocessingDirective, EndOfFile, Unknown, FaultyIdentifier
};

// 6.4.1
enum class KeyWord {
    Auto, Break, Case, Char, Const, Continue, Default, Do, Double, Else, Enum,
    Extern, Float, For, Goto, If, Inline, Int, Long, Register, Restrict,
    Return, Short, Signed, Sizeof, Static, Struct, Switch, Typedef, Union,
    Unsigned, Void, Volatile, While, Alignas, Alignof, Atomic, Bool, Complex,
    Generic, Imaginary, Noreturn, StaticAssert, ThreadLocal, None
};

// 6.4.6
enum class Punctuator {
    Question, LSquare, RSquare, LParen, RParen, LBrace, RBrace,
    Period, Ellipsis, Amp, AmpAmp, AmpEqual, Star, StarEqual,
    Plus, PlusPlus, PlusEqual, Minus, MinusMinus, MinusEqual, Arrow, 
    Tilde,Exclaim, ExclaimEqual, Slash,SlashEqual,Percent,PercentEqual, 
    Less, LessEqual, LessLess,  LessLessEqual, Greater, GreaterEqual, 
    GreaterGreater, GreaterGreaterEqual, Caret, CaretEqual, Pipe, PipePipe,
    PipeEqual, Colon, Semi, Equal, EqualEqual, Comma, Hash, HashHash, None
};

class TokenLoc {
    std::string fileName;
    size_t colCount;
    size_t lineCount;
public:
    TokenLoc()
    {
        fileName = "UNKNOWN";
        colCount = 0; 
        lineCount = 1; 
    }
    TokenLoc(std::string file, size_t col, size_t line)
    {
        fileName = std::move(file);
        colCount = col; 
        lineCount = line; 
    }
    size_t getColCount()    const {    return colCount;    }
    std::string toStr()  const {    return fileName + ":" + std::to_string(lineCount) + ":" + std::to_string(colCount);   }
};

class Token {
    TokenType token_type_;
    TokenLoc token_loc_;

    KeyWord keyword_;
    Punctuator punctuator_;
    std::string literalValue_;
    std::string redundant_part_;        
public:
    Token(TokenType token_type, TokenLoc token_loc, KeyWord keyword, Punctuator punctuator, std::string literalValue, std::string redundant_part)
    {
        token_type_ = token_type;
        token_loc_ = std::move(token_loc);
        keyword_ = keyword;
        punctuator_ = punctuator;
        literalValue_ = std::move(literalValue); 
        redundant_part_ = std::move(redundant_part);
    }

    Token(TokenType token_type, const TokenLoc& token_loc, KeyWord keyword) :
            Token(token_type, token_loc, keyword, Punctuator::None, "", ""){
        assert(token_type == TokenType::Keyword);
    }

    Token(TokenType token_type, TokenLoc token_loc, Punctuator punctuator) :
            Token(token_type, std::move(token_loc), KeyWord::None, punctuator, "", ""){
        assert(token_type == TokenType::Punctuator);
    }

    Token(TokenType token_type, TokenLoc token_loc, std::string literalValue, std::string redundant_part) :
            Token(token_type, std::move(token_loc), KeyWord::None, Punctuator::None, std::move(literalValue), std::move(redundant_part)){
        assert(token_type == TokenType::NumericConstantWithError || token_type == TokenType::FaultyIdentifier);
        if(token_type == TokenType::FaultyIdentifier)
        {
            std::string temp = literalValue_ + redundant_part_;
            literalValue_.clear();
            redundant_part_.clear();
            int first_legal_loc;
            for(int i = 0; i < temp.length(); i++)
            {
                if(temp[i] == '_' || isalpha(temp[i]))
                {
                    first_legal_loc = i;
                    break;
                }
            }
            redundant_part_ = temp.substr(0, first_legal_loc);
            literalValue_ = temp.substr(first_legal_loc, temp.length());
        }
    }

    Token(TokenType token_type, TokenLoc token_loc, std::string literalValue) :
            Token(token_type, std::move(token_loc), KeyWord::None, Punctuator::None, std::move(literalValue), ""){
        assert(token_type != TokenType::Keyword && token_type != TokenType::Punctuator \
                && token_type != TokenType::NumericConstantWithError && token_type != TokenType::FaultyIdentifier);
    }

    Token() : Token(TokenType::Unknown, TokenLoc(), KeyWord::None, Punctuator::None, "", "") {}

    TokenType getTokenType()            const {     return token_type_;        }
    TokenLoc getTokenLoc()              const {     return token_loc_;    }
    KeyWord getKeyWord()                const {     return keyword_;          }
    Punctuator getPunctuator()          const {     return punctuator_;       }
    std::string getLiteralValue()       const {     return literalValue_;     }

    std::string toStr() const;
    friend std::ostream& operator<<(std::ostream& os, const Token & token);
};


class Counter {
    size_t count_keyword;
    size_t count_identifier;
    size_t count_faulty_identifier;
    size_t count_numeric_constant;
    size_t count_numeric_constant_with_error;
    size_t count_character_constant;
    size_t count_string_literal;
    size_t count_punctuator;
    size_t count_preprocessing_directive;
    size_t count_EOF;
    size_t count_unknown;
public:
    void update(const Token& token);
    friend std::ostream& operator<<(std::ostream& os, const Counter & counter);
    size_t get_keyword() const                      {   return  count_keyword; }
    size_t get_numeric_constant() const             {   return  count_numeric_constant; }
    size_t get_identifier() const                   {   return  count_identifier; }
    size_t get_faulty_identifier() const            {   return  count_faulty_identifier; }
    size_t get_numeric_constantWithError() const    {   return  count_numeric_constant_with_error; }
    size_t get_character_constant() const           {   return  count_character_constant; }
    size_t get_string_literal() const               {   return  count_string_literal; }
    size_t get_punctuator()  const                  {   return  count_punctuator; }
    size_t get_preprocessing_directive() const      {   return  count_preprocessing_directive; }
    size_t get_EOF() const                          {   return  count_EOF; }
    size_t get_unknown()  const                     {   return  count_unknown; }
};

#endif //NAIVELEX_TOKEN_H