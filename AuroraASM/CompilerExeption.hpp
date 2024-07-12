#pragma once

#include <stdexcept>

class CompilerException : public std::runtime_error
{
public:
    CompilerException(const std::string& message, int line) : std::runtime_error(message), line(line) {}
    int line = 0;
};
