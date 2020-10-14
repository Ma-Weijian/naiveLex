#include "token.h"

const char *toTokenString(TokenType token_type) {
    switch (token_type) {
        case TokenType::Keyword:                    return "KeyWord";
        case TokenType::Identifier:                 return "Identifier";
        case TokenType::NumericConstant:            return "NumericConstant";
        case TokenType::NumericConstantWithError:   return "NumericConstantWithError";
        case TokenType::CharacterConstant:          return "CharacterConstant";
        case TokenType::StringLiteral:              return "StringLiteral";
        case TokenType::Punctuator:                 return "Punctuator";
        case TokenType::PreprocessingDirective:     return "PreprocessingDirective";
        case TokenType::EndOfFile:                  return "EndOfFile";
        case TokenType::Unknown:                    return "Unknown";
        default:                                    return "Unknown";
    };
}

const char *toKeywordString(KeyWord keyword) {
    switch (keyword) {
        case KeyWord::Auto:         return "auto";
        case KeyWord::Break:        return "break";
        case KeyWord::Case:         return "case";
        case KeyWord::Char:         return "char";
        case KeyWord::Const:        return "const";
        case KeyWord::Continue:     return "continue";
        case KeyWord::Default:      return "default";
        case KeyWord::Do:           return "do";
        case KeyWord::Double:       return "double";
        case KeyWord::Else:         return "else";
        case KeyWord::Enum:         return "enum";
        case KeyWord::Extern:       return "extern";
        case KeyWord::Float:        return "float";
        case KeyWord::For:          return "for";
        case KeyWord::Goto:         return "goto";
        case KeyWord::If:           return "if";
        case KeyWord::Inline:       return "inline";
        case KeyWord::Int:          return "int";
        case KeyWord::Long:         return "long";
        case KeyWord::Register:     return "register";
        case KeyWord::Restrict:     return "restrict";
        case KeyWord::Return:       return "return";
        case KeyWord::Short:        return "short";
        case KeyWord::Signed:       return "signed";
        case KeyWord::Sizeof:       return "sizeof";
        case KeyWord::Static:       return "static";
        case KeyWord::Struct:       return "struct";
        case KeyWord::Switch:       return "switch";
        case KeyWord::Typedef:      return "typedef";
        case KeyWord::Union:        return "union";
        case KeyWord::Unsigned:     return "unsigned";
        case KeyWord::Void:         return "void";
        case KeyWord::Volatile:     return "volatile";
        case KeyWord::While:        return "while";
        case KeyWord::Alignas:      return "alignas";
        case KeyWord::Alignof:      return "alignof";
        case KeyWord::Atomic:       return "atomic";
        case KeyWord::Bool:         return "bool";
        case KeyWord::Complex:      return "complex";
        case KeyWord::Generic:      return "generic";
        case KeyWord::Imaginary:    return "imaginary";
        case KeyWord::Noreturn:     return "noreturn";
        case KeyWord::StaticAssert: return "staticAssert";
        case KeyWord::ThreadLocal:  return "threadLocal";
        case KeyWord::None:         return "none";
        default:                    return "unknown";
    }
}

const char *toPunctuatorString(Punctuator punctuator) {
    switch (punctuator) {
        case Punctuator::Question:              return "?";
        case Punctuator::LSquare:               return "[";            case Punctuator::RSquare:                return "]";
        case Punctuator::LParen:                return "(";            case Punctuator::RParen:                 return ")";
        case Punctuator::LBrace:                return "{";            case Punctuator::RBrace:                 return "}";
        case Punctuator::Period:                return ".";            case Punctuator::Ellipsis:              return "...";
        case Punctuator::Amp:                   return "&";            case Punctuator::AmpAmp:                return "&&";            case Punctuator::AmpEqual:              return "&=";
        case Punctuator::Star:                  return "*";            case Punctuator::StarEqual:             return "*=";
        case Punctuator::Plus:                  return "+";            case Punctuator::PlusPlus:              return "++";            case Punctuator::PlusEqual:             return "+=";
        case Punctuator::Minus:                 return "-";            case Punctuator::MinusMinus:            return "--";            case Punctuator::MinusEqual:            return "-=";
        case Punctuator::Arrow:                 return "->";
        case Punctuator::Tilde:                 return "~";
        case Punctuator::Exclaim:               return "!";            case Punctuator::ExclaimEqual:          return "!=";
        case Punctuator::Slash:                 return "/";            case Punctuator::SlashEqual:            return "/=";
        case Punctuator::Percent:               return "%";            case Punctuator::PercentEqual:          return "%=";
        case Punctuator::Less:                  return "<";            case Punctuator::LessEqual:             return "<=";
        case Punctuator::LessLess:              return "<<";           case Punctuator::LessLessEqual:         return "<<=";
        case Punctuator::Greater:               return ">";            case Punctuator::GreaterEqual:          return ">=";
        case Punctuator::GreaterGreater:        return ">>";           case Punctuator::GreaterGreaterEqual:   return ">>=";
        case Punctuator::Caret:                 return "^";            case Punctuator::CaretEqual:            return "^=";
        case Punctuator::Pipe:                  return "|";            case Punctuator::PipePipe:              return "||";           case Punctuator::PipeEqual:             return "|=";
        case Punctuator::Colon:                 return ":";
        case Punctuator::Semi:                  return ";";
        case Punctuator::Equal:                 return "=";            case Punctuator::EqualEqual:            return "==";
        case Punctuator::Comma:                 return ",";
        case Punctuator::Hash:                  return "#";            case Punctuator::HashHash:              return "##";
        case Punctuator::None:                  return "None";
        default:                                return "Unknown";
    }
}

std::string Token::toStr() const {
    std::string result;
    result += "Type: ";
    result += std::string(toTokenString(this->token_type_)) + "\t\t";
    result += "Location: " + this->token_loc_.toStr() + "\t\t";
    result += "Value: ";
    switch(this->token_type_) {
        case TokenType::Keyword:
            result += std::string(toKeywordString(this->keyword_));
            break;
        case TokenType::Punctuator:
            result += std::string(toPunctuatorString(this->punctuator_));
            break;
        case TokenType::EndOfFile:
            break;
        default:
            result += this->literalValue_;
            break;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Token & token) {
    os << token.toStr();
    return os;
}
