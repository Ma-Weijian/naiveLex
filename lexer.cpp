#include "lexer.h"

Token naiveLex::getNextToken(TokenType prev_type) 
{
    /* refresh the bufffer. */
    token_buffer_.clear();
    /* means it has reached the end. */
    if(fileWrapper_.eof())
        return Token(TokenType::EndOfFile, fileWrapper_.getLocation(), "");
    
    /* get a char and judge if it is in the token. */
    auto c = fileWrapper_.getNextChar();
    /* This while skips all blank chars and get the following nonblank one. */
    while(isblank(c) || c == '\n')
    {
        if(fileWrapper_.eof())
            return Token(TokenType::EndOfFile, fileWrapper_.getLocation(), "");
        c = fileWrapper_.getNextChar();
    }
    currentLocation_ = fileWrapper_.getLocation();
    switch (c) 
    {
        //numbers
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            token_buffer_.push_back(c);
            return get_next_numeric_token(prev_type);
        //special alphabets which can mean both identifiers, numbers or string literals
        case 'L':
        case 'U':
            if(fileWrapper_.peekChar(1) == '\"') 
            {
                fileWrapper_.eatChars(1);
                return get_next_string_literial_token();
            } 
            else if(fileWrapper_.peekChar(1) == '\'') 
            {
                fileWrapper_.eatChars(1);
                return get_next_character_constant_token();
            } 
            else 
            {
                token_buffer_.push_back(c);
                return get_next_identifier_token();
            }
        case 'u':
            if(fileWrapper_.peekChar(1) == '\"') 
            {
                fileWrapper_.eatChars(1);
                return get_next_string_literial_token();
            } 
            else if(fileWrapper_.peekChar(1) == '\'') 
            {
                fileWrapper_.eatChars(1);
                return get_next_character_constant_token();
            } 
            else if((fileWrapper_.peekChar(1) == '8') && fileWrapper_.peekChar(2) == '\"') 
            {
                fileWrapper_.eatChars(2);
                return get_next_string_literial_token();
            } 
            else 
            {
                token_buffer_.push_back(c);
                return get_next_identifier_token();
            }
        //possible beginners of identifiers
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': /*'L'*/   case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':    /*'U'*/
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't':    /*'u'*/
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
            token_buffer_.push_back(c);
            return get_next_identifier_token();
        // Punctuators
        case '[':               return Token(TokenType::Punctuator, currentLocation_, Punctuator::LSquare);
        case ']':               return Token(TokenType::Punctuator, currentLocation_, Punctuator::RSquare);
        case '(':               return Token(TokenType::Punctuator, currentLocation_, Punctuator::LParen);
        case ')':               return Token(TokenType::Punctuator, currentLocation_, Punctuator::RParen);
        case '{':               return Token(TokenType::Punctuator, currentLocation_, Punctuator::LBrace);
        case '}':               return Token(TokenType::Punctuator, currentLocation_, Punctuator::RBrace);
        case '.':
            if(fileWrapper_.peekChar(1) == '.' && fileWrapper_.peekChar(2) == '.') 
            {
                fileWrapper_.eatChars(2);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Ellipsis);
            } 
            else if(isdigit(fileWrapper_.peekChar(1)))
            {
                token_buffer_.push_back(c);
                return get_next_numeric_token(prev_type);                                        //.85e+8
            }
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Period);
        case '-': 
        {
            switch (fileWrapper_.peekChar(1))
            {
                case '>':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::Arrow);
                case '-':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::MinusMinus);
                case '=':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::MinusEqual);
                default:
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::Minus);
            }
        }
        case '+':
        {
            switch (fileWrapper_.peekChar(1)) 
            {
                case '+':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::PlusPlus);
                case '=':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::PlusEqual);
                default:
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::Plus);
            }
        }
        case '&': 
        {
            switch (fileWrapper_.peekChar(1))
            {
                case '&':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::AmpAmp);
                case '=':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::AmpEqual);
                default:
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::Amp);
            }
        }
        case '*':
            if(fileWrapper_.peekChar(1) == '=')
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::StarEqual);
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Star);
        case '~':                   return Token(TokenType::Punctuator, currentLocation_, Punctuator::Tilde);
        case '!':
            if(fileWrapper_.peekChar(1) == '=')
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::ExclaimEqual);
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Exclaim);
        case '/':
            if(fileWrapper_.peekChar(1) == '/')
            {
                fileWrapper_.eatChars(1);
                skip_line_comment();
                return getNextToken(prev_type);
            } 
            else if(fileWrapper_.peekChar(1) == '*')
            {
                fileWrapper_.eatChars(1);
                skip_block_comment();
                return getNextToken(prev_type);
            } 
            else if(fileWrapper_.peekChar(1) == '=')
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::SlashEqual);
            }
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Slash);
        case '%':
            if(fileWrapper_.peekChar(1) == '=') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::PercentEqual);
            }
            else if(fileWrapper_.peekChar(1) == '>')
            {
                fileWrapper_ .eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::RBrace);
            } 
            else if(fileWrapper_.peekChar(1) == ':')
            {
                if(fileWrapper_.peekChar(2) == '%' && fileWrapper_.peekChar(3) == ':') 
                {
                    fileWrapper_.eatChars(3);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::HashHash);
                } 
                else 
                {
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::Hash);
                }
            }
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Percent);
        case '<':
            if(fileWrapper_.peekChar(1) == '<') 
            {
                if(fileWrapper_.peekChar(2) == '=') 
                {
                    fileWrapper_.eatChars(2);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::LessLessEqual);
                } 
                else 
                {
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::LessLess);
                }
            } 
            else if(fileWrapper_.peekChar(1) == '=') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::LessEqual);
            } 
            else if(fileWrapper_.peekChar(1) == ':') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::LSquare);
            } 
            else if(fileWrapper_.peekChar(1) == '%')
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::LBrace);
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Less);
        case '>':
            if(fileWrapper_.peekChar(1) == '>')
            {
                if(fileWrapper_.peekChar(2) == '=') 
                {
                    fileWrapper_.eatChars(2);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::GreaterGreaterEqual);
                } 
                else if(fileWrapper_.peekChar(1) == '=') 
                {
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::GreaterEqual);
                } 
                else 
                {
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::GreaterGreater);
                }
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::GreaterGreater);
        case '=':
            if(fileWrapper_.peekChar(1) == '=') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::EqualEqual);
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Equal);
        case '^':
            if(fileWrapper_.peekChar(1) == '=') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::CaretEqual);
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Caret);
        case '|': 
        {
            switch (fileWrapper_.peekChar(1)) 
            {
                case '|':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::PipePipe);
                case '=':
                    fileWrapper_.eatChars(1);
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::PipeEqual);
                default:
                    return Token(TokenType::Punctuator, currentLocation_, Punctuator::Pipe);
            }
        }
        case '?':           return Token(TokenType::Punctuator, currentLocation_, Punctuator::Question);
        case ':':
            if(fileWrapper_.peekChar(1) == '>') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::RSquare);
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Colon);
        case ';':            return Token(TokenType::Punctuator, currentLocation_, Punctuator::Semi);
        case ',':            return Token(TokenType::Punctuator, currentLocation_, Punctuator::Comma);
        case '#':
            if(fileWrapper_.peekChar(1) == '#') 
            {
                fileWrapper_.eatChars(1);
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::HashHash);
            } 
            else if(currentLocation_.getColCount() == 1) 
            {
                return get_next_preprocessing_directive_token();
            } 
            else
                return Token(TokenType::Punctuator, currentLocation_, Punctuator::Hash);
        case '\"':
            return get_next_string_literial_token();
        case '\'':
            return get_next_character_constant_token();
        default:
            token_buffer_.push_back(c);
            break;
    }
    auto result = Token(TokenType::Unknown, fileWrapper_.getLocation(), token_buffer_);
    return result;
}

Token naiveLex::get_next_preprocessing_directive_token() 
{
    char c = '*';
    while(!fileWrapper_.eof() && (fileWrapper_.peekChar(1) != '\n' || c == '\\')) 
    {
        c = fileWrapper_.getNextChar();
        token_buffer_.push_back(c);
    }
    return Token(TokenType::PreprocessingDirective, currentLocation_, token_buffer_);
}

void naiveLex::skip_line_comment() 
{
    while(!fileWrapper_.eof() && fileWrapper_.getNextChar() != '\n');
}

void naiveLex::skip_block_comment() 
{
    while(!fileWrapper_.eof()) 
    {
        if(fileWrapper_.getNextChar() == '*' && fileWrapper_.peekChar(1) == '/') 
        {
            fileWrapper_.eatChars(1);
            return;
        }
    }
}

bool isPermittedNumericToken(const char c) 
{
    return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ||
            (c == 'x') || (c == 'X') || (c == 'p') || (c == 'P') || (c == 'l') || (c == 'L') || (c == 'u') || (c == 'U') || (c == '.');
}

/* 0.1f, 13ll, 22ull etc..*/
bool isPermittedNumericSuffix(const char c) 
{
    return c == 'f' || c == 'F' || c == 'L' || c == 'l';
}

bool isPermittedNumericSuffix(const char c1, const char c2) 
{
    return (c1 == 'L' && c2 == 'L') || (c1 == 'l' && c2 == 'l') || ((c1 == 'u' || c1 == 'U') && (c2 == 'L' || c2 == 'l'));
}

bool isPermittedNumericSuffix(const char c1, const char c2, const char c3) 
{
    return ((c1 == 'u' || c1 == 'U') && ((c2 == 'L' && c3 == 'L') || (c2 == 'l' && c3 == 'l')));
}

bool isLegalAfterNum(char c)
{
    return isspace(c) || c == EOF || c == '-' || c == '-' || c == '+' || c == '&' || c == '*' || c == '/' || c == '%' || \
            c == '>' || c == '<' || c == '=' || c == '^' || c == '|' || c == '?' || c == ':' || c == ';' || c == ',' || c == ']' || c == ')';
}

Token naiveLex::get_next_numeric_token(TokenType prev_type) 
{
    while(!fileWrapper_.eof() && isPermittedNumericToken(fileWrapper_.peekChar(1))) 
    {
        if((fileWrapper_.peekChar(1) == 'e' || fileWrapper_.peekChar(1) == 'E' ||               //e.g. 1.234567e-8 or 0x34a.bdp+9
                fileWrapper_.peekChar(1) == 'p' || fileWrapper_.peekChar(1) == 'P') &&          //we should get one more char at this case.
                (fileWrapper_.peekChar(2) == '+' || fileWrapper_.peekChar(2) == '-')) 
                {
                    token_buffer_.push_back(fileWrapper_.getNextChar());
                }
        token_buffer_.push_back(fileWrapper_.getNextChar());
    }

    std::string faulty_suffix;
    while (!isLegalAfterNum(fileWrapper_.peekChar(1)))
    {
        faulty_suffix.push_back(fileWrapper_.getNextChar());
    }
     
    size_t offset;
    std::stof(token_buffer_, &offset);
    if(offset == token_buffer_.size() ||
        (offset + 1 == token_buffer_.size() && isPermittedNumericSuffix(token_buffer_.at(offset))) ||
            (offset + 2 == token_buffer_.size() && isPermittedNumericSuffix(token_buffer_.at(offset), token_buffer_.at(offset + 1))) ||
                (offset + 3 == token_buffer_.size() && isPermittedNumericSuffix(token_buffer_.at(offset), token_buffer_.at(offset + 1), token_buffer_.at(offset + 2)))) 
                {
                    if(faulty_suffix.empty())
                        return Token(TokenType::NumericConstant, currentLocation_, token_buffer_);
                }
    if(prev_type == TokenType::Keyword)
        return Token(TokenType::FaultyIdentifier, currentLocation_, token_buffer_, faulty_suffix);      //We first move them in and do the handling later.
    else
        return Token(TokenType::NumericConstantWithError, currentLocation_, token_buffer_, faulty_suffix);
}

/* We scan until the next \" */
Token naiveLex::get_next_string_literial_token() 
{
    char c;
    while(!fileWrapper_.eof() && (c = fileWrapper_.getNextChar()) != '\"') 
    {
        token_buffer_.push_back(c);
    }
    return Token(TokenType::StringLiteral, currentLocation_, token_buffer_);
}

Token naiveLex::get_next_character_constant_token() 
{
    char c;
    while(!fileWrapper_.eof() && (c = fileWrapper_.getNextChar()) != '\'') 
    {
        token_buffer_.push_back(c);
    }
    return Token(TokenType::CharacterConstant, currentLocation_, token_buffer_);
}

bool isPermittedChar(const char c) 
{
    return c == '_' || isalnum(c);
}

Token naiveLex::get_next_identifier_token() 
{
    while(!fileWrapper_.eof() && isPermittedChar(fileWrapper_.peekChar(1))) 
    {
        token_buffer_.push_back(fileWrapper_.getNextChar());
    }
    if(token_buffer_ == "auto")                      return Token(TokenType::Keyword, currentLocation_, KeyWord::Auto);
    else if(token_buffer_ == "break")                return Token(TokenType::Keyword, currentLocation_, KeyWord::Break);
    else if(token_buffer_ == "case")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Case);
    else if(token_buffer_ == "char")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Char);
    else if(token_buffer_ == "const")                return Token(TokenType::Keyword, currentLocation_, KeyWord::Const);
    else if(token_buffer_ == "continue")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Continue);
    else if(token_buffer_ == "default")              return Token(TokenType::Keyword, currentLocation_, KeyWord::Default);
    else if(token_buffer_ == "do")                   return Token(TokenType::Keyword, currentLocation_, KeyWord::Do);
    else if(token_buffer_ == "double")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Double);
    else if(token_buffer_ == "else")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Else);
    else if(token_buffer_ == "enum")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Enum);
    else if(token_buffer_ == "extern")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Extern);
    else if(token_buffer_ == "float")                return Token(TokenType::Keyword, currentLocation_, KeyWord::Float);
    else if(token_buffer_ == "for")                  return Token(TokenType::Keyword, currentLocation_, KeyWord::For);
    else if(token_buffer_ == "goto")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Goto);
    else if(token_buffer_ == "if")                   return Token(TokenType::Keyword, currentLocation_, KeyWord::If);
    else if(token_buffer_ == "inline")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Inline);
    else if(token_buffer_ == "int")                  return Token(TokenType::Keyword, currentLocation_, KeyWord::Int);
    else if(token_buffer_ == "long")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Long);
    else if(token_buffer_ == "register")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Register);
    else if(token_buffer_ == "restrict")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Restrict);
    else if(token_buffer_ == "return")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Return);
    else if(token_buffer_ == "short")                return Token(TokenType::Keyword, currentLocation_, KeyWord::Short);
    else if(token_buffer_ == "signed")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Signed);
    else if(token_buffer_ == "sizeof")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Sizeof);
    else if(token_buffer_ == "static")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Static);
    else if(token_buffer_ == "struct")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Struct);
    else if(token_buffer_ == "switch")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Switch);
    else if(token_buffer_ == "typeof")               return Token(TokenType::Keyword, currentLocation_, KeyWord::Typedef);
    else if(token_buffer_ == "union")                return Token(TokenType::Keyword, currentLocation_, KeyWord::Union);
    else if(token_buffer_ == "unsigned")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Unsigned);
    else if(token_buffer_ == "void")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::Void);
    else if(token_buffer_ == "volatile")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Volatile);
    else if(token_buffer_ == "while")                return Token(TokenType::Keyword, currentLocation_, KeyWord::While);
    else if(token_buffer_ == "_Alignas")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Alignas);
    else if(token_buffer_ == "_Alignof")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Alignof);
    else if(token_buffer_ == "_Atomic")              return Token(TokenType::Keyword, currentLocation_, KeyWord::Atomic);
    else if(token_buffer_ == "_Bool")                return Token(TokenType::Keyword, currentLocation_, KeyWord::Bool);
    else if(token_buffer_ == "_Complex")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Complex);
    else if(token_buffer_ == "_Generic")             return Token(TokenType::Keyword, currentLocation_, KeyWord::Generic);
    else if(token_buffer_ == "_Imaginary")           return Token(TokenType::Keyword, currentLocation_, KeyWord::Imaginary);
    else if(token_buffer_ == "_Noreturn")            return Token(TokenType::Keyword, currentLocation_, KeyWord::Noreturn);
    else if(token_buffer_ == "_Static_assert")       return Token(TokenType::Keyword, currentLocation_, KeyWord::StaticAssert);
    else if(token_buffer_ == "_Thread_local")        return Token(TokenType::Keyword, currentLocation_, KeyWord::ThreadLocal);
    else if(token_buffer_ == "None")                 return Token(TokenType::Keyword, currentLocation_, KeyWord::None);
    return Token(TokenType::Identifier, currentLocation_, token_buffer_);
}
