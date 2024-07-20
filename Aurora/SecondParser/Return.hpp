#pragma once

#include "Expression/Expression.hpp"

class Return : public Expression
{
public:
    Return();
    ~Return();
};

Return::Return() : Expression(Expression::Type::Void)
{

}

Return::~Return()
{

}
