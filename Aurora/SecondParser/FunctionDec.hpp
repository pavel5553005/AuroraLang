#pragma once

#include "Expression/Variable.hpp"

class FunctionDec
{
public:
    std::vector<Expression> args;
    FunctionDec();
    ~FunctionDec();
};

FunctionDec::FunctionDec()
{
}

FunctionDec::~FunctionDec()
{
}
