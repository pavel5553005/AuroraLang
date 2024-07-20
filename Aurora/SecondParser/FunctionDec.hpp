#pragma once

#include "Expression/Variable.hpp"

class FunctionDec : public Expression
{
public:
    std::vector<VariableDec*> args;
    std::string name;
    FunctionDec(Lexer::LineWithTokens line);
    ~FunctionDec();
};

FunctionDec::FunctionDec(Lexer::LineWithTokens line) : Expression(getTypeFromToken(line.t[0]))
{
    if (line.t.size() < 4) throw ParserExeption("Invalid declaration for function", line.line);
    name = line.t[1].value;
    for (auto it = line.t.begin() + 3; it != line.t.end(); it++)
    {
        VariableDec* e = new VariableDec(Expression::Type::Void, "");
        if (it->type != Token::Type::Type) throw ParserExeption("Expected type", line.line);
        e->type = getTypeFromToken(*it);
        it++;
        if (it == line.t.end() or it->type != Token::Type::Identifier) throw ParserExeption("Expected identifier", line.line);
        e->name = it->value;
        it++;
        if (it == line.t.end() or (it->type != Token::Type::RightParen and it->type != Token::Type::Comma)) throw ParserExeption("Expected ')' or ','", line.line);
        args.push_back(e);
        children.push_back(e);
    }
}

FunctionDec::~FunctionDec()
{
}
