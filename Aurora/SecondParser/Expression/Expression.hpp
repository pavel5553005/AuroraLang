#pragma once

#include "../Node.hpp"

class Expression : public Node
{
public:
    enum Type
    {
        Bool = 0,
        Char = 1,
        Short = 2,
        Int = 3,
        Long = 4,
        Float = 5,
        Double = 6,
        Void = 7
    };
    Type type;

    static Expression::Type getTypeFromToken(Token t);
    static Expression::Type getTypeFromString(std::string s);

    Expression(Type type);
    ~Expression();
};

Expression::Expression(Type type) : type(type)
{
}

Expression::~Expression()
{
}

Expression::Type Expression::getTypeFromToken(Token t)
{
    if (t.type == Token::Type::Type)
    {
        if (t.value == "bool")
        {
            return Expression::Type::Bool;
        }
        else if (t.value == "char")
        {
            return Expression::Type::Char;
        }
        else if (t.value == "short")
        {
            return Expression::Type::Short;
        }
        else if (t.value == "int")
        {
            return Expression::Type::Int;
        }
        else if (t.value == "long")
        {
            return Expression::Type::Long;
        }
        else if (t.value == "float")
        {
            return Expression::Type::Float;
        }
        else if (t.value == "double")
        {
            return Expression::Type::Double;
        }
    }
    return Expression::Type::Void;
}

Expression::Type Expression::getTypeFromString(std::string s)
{
    auto isInteger = [](const std::string& s) -> bool {
        for (char c : s) {
            if (!std::isdigit(c) && c != '+' && c != '-') {
                return false;
            }
        }
        return true;
    };

    // Проверяем, является ли строка double
    auto isDouble = [](const std::string& s) -> bool {
        bool pointSeen = false;
        for (char c : s) {
            if (!std::isdigit(c) && c != 'e' && c != 'E' && c != '+' && c != '-' && c != '.') {
                return false;
            }
            if (c == '.') {
                if (pointSeen) return false;
                pointSeen = true;
            }
        }
        return pointSeen;
    };

    // Проверяем, является ли строка float
    auto isFloat = [isDouble](const std::string& s) -> bool {
        if (s.back() != 'f' && s.back() != 'F') return false;
        return isDouble(s.substr(0, s.size() - 1));
    };

    // Проверяем, является ли строка bool
    auto isBool = [](const std::string& s) -> bool {
        return s == "true" || s == "false";
    };

    if (isBool(s)) return Expression::Type::Bool;
    if (isInteger(s)) return Expression::Type::Int;
    if (isDouble(s)) return Expression::Type::Double;
    if (isFloat(s)) return Expression::Type::Float;
    throw ParserExeption("Unknown type", -1);
}

