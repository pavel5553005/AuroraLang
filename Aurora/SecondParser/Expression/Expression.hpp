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
    Expression(Type type);
    ~Expression();
};

Expression::Expression(Type type) : type(type)
{
}

Expression::~Expression()
{
}
