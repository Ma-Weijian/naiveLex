#include "file.h"

std::ostream& operator<<(std::ostream& os, FileWrapper & fileWrapper) 
{
    os << fileWrapper.getName() << ":" << fileWrapper.getLineCount() << ":" << fileWrapper.getColumn();
    return os;
}

TokenLoc FileWrapper::getLocation() 
{
    return TokenLoc(fileName_, column_, line_);
}