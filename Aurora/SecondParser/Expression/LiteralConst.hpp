#pragma once

#include "Expression.hpp"

class LiteralConst : public Expression
{
public:
    std::string value;
    LiteralConst(std::string value);
    ~LiteralConst();
};

LiteralConst::LiteralConst(std::string value) : Expression(Expression::Type::Void), value(value) { }

LiteralConst::~LiteralConst() { }
