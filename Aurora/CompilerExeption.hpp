#pragma once

#include <stdexcept>

class LexerExeption : public std::runtime_error
{
public:
    int line;
    int column;
    LexerExeption(std::string message, int line, int column) : std::runtime_error(message), line(line), column(column) {}
};

class ParserExeption : public std::runtime_error
{
public:
    int line;
    ParserExeption(std::string message, int line) : std::runtime_error(message), line(line) {}
};
