#pragma once

#include "Expression/VariableDec.hpp"

class Assignment : public Node
{
public:
    Assignment() {}
    ~Assignment() {}
    VariableDec* variable;
};
