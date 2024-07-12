#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "Constans.hpp"
#include "../CompilerExeption.hpp"

class Token
{
public:
    enum class Type
    {
        LiteralConstant,
        Keyword,
        Type,
        Identifier,
        Operator,
        Point,
        Colon,
        Comma,
        Semicolon,
        LeftParen,
        RightParen,
        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        Comment
    };
    
    Type type;
    std::string value;

    Token(std::string value, Type type);
    ~Token();
};

Token::Token(std::string value, Type type) : type(type), value(value) { }

Token::~Token() { }
