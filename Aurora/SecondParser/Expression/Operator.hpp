#pragma once

#include "Expression.hpp"

const std::vector<std::vector<std::string>> OperatorList = {
    {">", "<", ">=", "<=", "==", "!="},
    {"+", "-"},
    {"*", "/", "%"}
};

class Operator : public Expression
{
public:
    enum Type
    {
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Gre,
        Less,
        GreEq,
        LessEq,
        Eq,
        NotEq
    };
    Type opType;
    Operator(std::string value);
    ~Operator();
};

Operator::Operator(std::string value) : Expression(Expression::Type::Void), opType()
{
    if (value == "+")
    {
        opType = Add;
    }
    else if (value == "-")
    {
        opType = Sub;
    }
    else if (value == "*")
    {
        opType = Mul;
    }
    else if (value == "/")
    {
        opType = Div;
    }
    else if (value == "%")
    {
        opType = Mod;
    }
    else if (value == ">")
    {
        opType = Gre;
    }
    else if (value == "<")
    {
        opType = Less;
    }
    else if (value == ">=")
    {
        opType = GreEq;
    }
    else if (value == "<=")
    {
        opType = LessEq;
    }
    else if (value == "==")
    {
        opType = Eq;
    }
    else if (value == "!=")
    {
        opType = NotEq;
    }
    else
    {
        throw ParserExeption("Unknown operator: " + value, -1);
    }
}

Operator::~Operator()
{
}
