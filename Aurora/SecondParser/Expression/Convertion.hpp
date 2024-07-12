#pragma once

#include "Expression.hpp"

class Convertion : public Expression
{
public:
    Expression::Type from;
    bool isExplicit = false;
    static std::string getStringFromType(Expression::Type type);
    Convertion(Expression::Type to, Expression::Type from, bool isExplicit = false);
    ~Convertion();
};

Convertion::Convertion(Expression::Type to, Expression::Type from, bool isExplicit) : Expression(to), from(from), isExplicit(isExplicit)
{
    if (from == Expression::Type::Void or to == Expression::Type::Void) throw ParserExeption("Invalid convertion " + getBlue(Convertion::getStringFromType(from)) + " to " + getBlue(Convertion::getStringFromType(to)), -1);
    if (from > to and isExplicit == false)
    {
        throw ParserExeption("Invalid convertion " + getBlue(Convertion::getStringFromType(from)) + " to " + getBlue(Convertion::getStringFromType(to)), -1);
    }
}

std::string Convertion::getStringFromType(Expression::Type type)
{
    switch (type)
    {
    case Expression::Type::Bool: return "bool";
    case Expression::Type::Char: return "char";
    case Expression::Type::Short: return "short";
    case Expression::Type::Int: return "int";
    case Expression::Type::Long: return "long";
    case Expression::Type::Float: return "float";
    case Expression::Type::Double: return "double";
    case Expression::Type::Void: return "void";
    }
    return "";
}

Convertion::~Convertion()
{
}
