#pragma once

#include "Expression.hpp"

class Variable : public Expression
{
public:
    Variable() : Expression(Expression::Type::Void) {}
    ~Variable() {}
    std::string name;
};

