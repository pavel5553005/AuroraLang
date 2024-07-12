#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Token.hpp"

class Lexer
{
public:
    struct LineWithTokens
    {
        std::vector<Token> t;
        int line;
        int spaceCount;
    };
    typedef std::vector<Lexer::LineWithTokens> Tokanized;
private:
    

    Tokanized tokens;
    std::vector<Token> t;

    std::vector<std::string> lines;

    int index = 0;
    int line = 0;

    void space();
    bool type();
    bool keyword();
    bool operator_();
    bool sign();
    void comment();
    bool literalConstant();
    bool number();
    bool identifier();

    void addToken(Token tok) { t.push_back(tok); index++; }

    Token nextToken();

    void tokenizeLine();

public:
    Lexer(std::string& scr);

    Tokanized& getTokens();

    ~Lexer();
};

Lexer::Lexer(std::string& scr)
{
    std::string tmp = "";
    for (auto c : scr)
    {
        if (c == '\n')
        {
            this->lines.push_back(tmp);
            tmp = "";
        }
        else
        {
            tmp += c;
        }
    }
}


Lexer::Tokanized& Lexer::getTokens()
{
    if (lines.size() == 0)
    {
        throw LexerExeption("Empty code", 0, 0);
    }
    while (line < lines.size())
    {
        if (lines[line][index] == '\n')
        {
            line++;
            index = 0;
        }
        else
        {
            try
            {
                tokenizeLine();
                line++;
                index = 0;
            }
            catch(const std::exception& e)
            {
                throw LexerExeption(e.what(), line, index);
            }
        }
    }
    return tokens;
}

bool Lexer::keyword()
{
    for (auto keyword : Keywords)
    {
        if (lines[line].substr(index, keyword.length()) == keyword and !isalnum(lines[line][index + keyword.length()]))
        {
            t.push_back(Token(keyword, Token::Type::Keyword));
            index += keyword.length();
            return true;
        }
    }
    return false;
}

bool Lexer::type()
{
    for (auto type : Types)
    {
        if (lines[line].substr(index, type.length()) == type and !isalnum(lines[line][index + type.length()]))
        {
            t.push_back(Token(type, Token::Type::Type));
            index += type.length();
            return true;
        }
    }
    return false;
}

bool Lexer::operator_()
{
    for (auto oper : Operators)
    {
        if (lines[line].substr(index, oper.length()) == oper)
        {
            t.push_back(Token(oper, Token::Type::Operator));
            index += oper.length();
            return true;
        }
    }
    return false;
}

bool Lexer::sign()
{
    switch (lines[line][index])
    {
    case ';':
        addToken(Token(";", Token::Type::Semicolon));
        return true;
    case '(':
        addToken(Token("(", Token::Type::LeftParen));
        return true;
    case ')':
        addToken(Token(")", Token::Type::RightParen));
        return true;
    case '{':
        addToken(Token("{", Token::Type::LeftBrace));
        return true;
    case '}':
        addToken(Token("}", Token::Type::RightBrace));
        return true;
    case '[':
        addToken(Token("[", Token::Type::LeftBracket));
        return true;
    case ']':
        addToken(Token("]", Token::Type::RightBracket));
        return true;
    case ',':
        addToken(Token(",", Token::Type::Comma));
        return true;
    case ':':
        addToken(Token(":", Token::Type::Colon));
        return true;
    case '.':
        addToken(Token(".", Token::Type::Point));
        return true;
    default:
        return false;
    }
}

void Lexer::comment()
{
    if (lines[line].substr(index, 2) == "//")
    {
        while (lines[line][index] != '\n')
        {
            index++;
        }
        line++;
        index = 0;
    }
    else if (lines[line].substr(index, 2) == "/*")
    {
        while (lines[line].substr(index, 2) != "*/")
        {
            index++;
            if (index && lines[line][index] == '\n')
            {
                line++;
                index = 0;
            }
        }
    }
}

bool Lexer::literalConstant()
{
    if (lines[line][index] == '\'')
    {
        index++;
        addToken(Token(lines[line].substr(index, 1), Token::Type::LiteralConstant));
        return true;
    }
    else if (lines[line][index] == '"')
    {
        std::string value = "";
        index++;
        while (lines[line][index] != '"')
        {
            value += lines[line].substr(index, 1);
            index++;
        }
        addToken(Token(value, Token::Type::LiteralConstant));
        return true;
    }
    return false;
}

bool Lexer::number()
{
    std::string value = "";
    if (isdigit(lines[line][index]))
    {
        if (lines[line].substr(index, 2) == "0x" or lines[line].substr(index, 2) == "0X")
        {
            index += 2;
            while (isxdigit(lines[line][index]))
            {
                value += lines[line].substr(index, 1);
                index++;
            }
            t.push_back(Token(std::to_string(stoi(value, nullptr, 16)), Token::Type::LiteralConstant));
            return true;
        }
        if (lines[line].substr(index, 2) == "0b" or lines[line].substr(index, 2) == "0B")
        {
            index += 2;
            while (lines[line][index] == '0' or lines[line][index] == '1')
            {
                value += lines[line].substr(index, 1);
                index++;
            }
            t.push_back(Token(std::to_string(stoi(value, nullptr, 2)), Token::Type::LiteralConstant));
            return true;
        }
        while (isdigit(lines[line][index]) or lines[line][index] == '.')
        {
            value += lines[line].substr(index, 1);
            index++;
        }
        if (lines[line][index] == 'f')
        {
            index++;
            t.push_back(Token(value + "f", Token::Type::LiteralConstant));
            return true;
        }
        t.push_back(Token(value, Token::Type::LiteralConstant));
        return true;
    }
    return false;
}

bool Lexer::identifier()
{
    std::string value = "";
    while (isalnum(lines[line][index]))
    {
        value += lines[line].substr(index, 1);
        index++;
    }
    if (value.length() == 0 or isdigit(value[0]) or value[0] == '_')
        throw std::runtime_error("Unknown token: " + blue + value + reset);
    t.push_back(Token(value, Token::Type::Identifier));
    return true;
}

void Lexer::space()
{
    while (isspace(lines[line][index]))
    {
        index++;
    }
}

void Lexer::tokenizeLine()
{
    int countOfSpaces = 0;
    while (lines[line][index] == ' ' or lines[line][index] == '\t')
    {
        index++;
        countOfSpaces++;
    }
    while (index < lines[line].length())
    {
        space();
        if (type()) continue;
        if (keyword()) continue;
        if (operator_()) continue;
        if (sign()) continue;
        comment();
        if (literalConstant()) continue;
        if (number()) continue;
        if (identifier()) continue;
    }
    if (t.size() > 0)
    {
        tokens.push_back({t, line, countOfSpaces});
        if (t.front().value != "for" and t.back().type == Token::Type::Semicolon) throw std::runtime_error("This is not c++, semicolons are not welcome here :)");
    }
    t.clear();
}

Lexer::~Lexer() {}
