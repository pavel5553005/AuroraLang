#pragma once

#include <string>
#include <vector>
#include "CompilerExeption.hpp"

enum TokenType
{
    Comma,
    Immediate,
    String,
    Label,
    Syscall,

    AddressImmediate,
    AddressRegister,
    Variable,

    Register,
    Instruction,
    Var,
    Segment,
    Plus,
    Minus
};

struct Token
{
    TokenType type;
    std::string value;
};

struct LineWithTokens
{
    std::vector<Token> tokens;
    int line;
};

typedef std::vector<LineWithTokens> TokanizedFile;

enum ArgumentType
{
    Imm,

    RegRx,
    RegBr,
    RegTbr,
    RegFbr,
    RegSp,
    RegBp,

    AddrImm,
    AddrRegRx
};
