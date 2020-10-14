//#include "counter.h"
#include "token.h"


void Counter::update(const Token &token)
{
    switch (token.getTokenType())
    {
        case TokenType::Keyword:                    count_keyword++;                    return;
        case TokenType::Identifier:                 count_identifier++;                 return;
        case TokenType::NumericConstant:            count_numeric_constant++;            return;
        case TokenType::NumericConstantWithError:   count_numeric_constant_with_error++;   return;
        case TokenType::CharacterConstant:          count_character_constant++;          return;
        case TokenType::StringLiteral:              count_string_literal++;              return;
        case TokenType::Punctuator:                 count_punctuator++;                 return;
        case TokenType::PreprocessingDirective:     count_preprocessing_directive++;     return;
        case TokenType::EndOfFile:                  count_EOF++;                  return;
        case TokenType::Unknown:                    count_unknown++;                    return;
        default:                                                                        return;
    }
}

std::ostream& operator<<(std::ostream& os, const Counter & counter) 
{
    os << "KeyWord"  << ":\t" << counter.get_keyword() << std::endl;
    os << "Identifier"  << ":\t" << counter.get_identifier() << std::endl;
    os << "NumericConstant"  << ":\t" << counter.get_numeric_constant() << std::endl;
    os << "NumericConstant(Error Detected)"  << ":\t" << counter.get_numeric_constantWithError() << std::endl;
    os << "CharacterConstant"  << ":\t" << counter.get_character_constant() << std::endl;
    os << "StringLiteral"  << ":\t" << counter.get_string_literal() << std::endl;
    os << "Punctuator"  << ":\t" << counter.get_punctuator() << std::endl;
    os << "PreprocessingDirective"  << ":\t" << counter.get_preprocessing_directive() << std::endl;
    os << "EndOfFile"  << ":\t" << counter.get_EOF() << std::endl;
    os << "Unknown"  << ":\t" << counter.get_unknown() << std::endl;
    return os;
}
