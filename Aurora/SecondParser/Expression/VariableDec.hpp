#pragma once

#include "Expression.hpp"

class VariableDec : public Expression
{
public:
    std::string name;
    bool isInitialized = false;
    VariableDec(Expression::Type type, std::string name);
    ~VariableDec();
};

VariableDec::VariableDec(Expression::Type type, std::string name) : Expression(type), name(name) { }

VariableDec::~VariableDec() { }
