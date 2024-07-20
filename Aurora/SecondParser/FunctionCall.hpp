#pragma once

#include "Expression/Expression.hpp"

class FunctionCall : public Expression
{
public:
    std::string name;
    FunctionCall();
    ~FunctionCall();
};

FunctionCall::FunctionCall() : Expression(Expression::Type::Void)
{
}

FunctionCall::~FunctionCall()
{
}
