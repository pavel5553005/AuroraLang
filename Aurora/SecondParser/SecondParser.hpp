#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../FirstParser/FirstParser.hpp"
#include "Node.hpp"
#include "Expression/Expression.hpp"
#include "Expression/VariableDec.hpp"
#include "Expression/LiteralConst.hpp"
#include "Expression/Operator.hpp"
#include "Expression/Convertion.hpp"
#include "Cycles/While.hpp"
#include "Expression/Variable.hpp"
#include "Assignment.hpp"
#include "Cycles/For.hpp"
#include "Cycles/DoWhile.hpp"
#include "If.hpp"
#include "Else.hpp"
#include "ElseIf.hpp"
#include "FunctionDec.hpp"
#include "FunctionCall.hpp"

class SecondParser
{
private:
    FirstParser::Node& nodes;
    Expression* root;
    std::vector<VariableDec*> variables;
    std::vector<FunctionDec*> functions;

    Expression* parseExpression(Lexer::LineWithTokens line, int level);
    
    VariableDec* parseVariableDec(Lexer::LineWithTokens& line);
    Assignment* parseAssignment(Lexer::LineWithTokens& line);
    Node* parseOperation(Lexer::LineWithTokens& line);

    bool isOperatorInList(std::string op, int level);

    bool isFunctionDec(Lexer::LineWithTokens& line);
    bool isVariableDec(Lexer::LineWithTokens& line);
    bool isAssignment(Lexer::LineWithTokens& line);
    bool isForLoop(Lexer::LineWithTokens& line);
    bool isWhileLoop(Lexer::LineWithTokens& line);
    bool isIf(Lexer::LineWithTokens& line);

    VariableDec* findIdentifier(std::string name);
public:
    void findFunctionDecs();
    std::vector<Node*> parse(std::vector<FirstParser::Node>& nodes);
    SecondParser(FirstParser::Node& nodes);
    ~SecondParser();
};

SecondParser::SecondParser(FirstParser::Node& nodes) : nodes(nodes)
{
    
}
SecondParser::~SecondParser()
{
}

void SecondParser::findFunctionDecs()
{
    for (int i = 0; i < nodes.children.size(); i++)
    {
        if (isFunctionDec(nodes.children[i].line))
        {
            functions.push_back(new FunctionDec(nodes.children[i].line));
        }
    }
}

bool SecondParser::isOperatorInList(std::string op, int level)
{
    for (int i = 0; i < OperatorList[level].size(); i++)
    {
        if (op == OperatorList[level][i]) return true;
    }
    return false;
}

bool SecondParser::isFunctionDec(Lexer::LineWithTokens& line)
{
    if (line.t.size() >= 3 and line.t[0].type == Token::Type::Type and line.t[1].type == Token::Type::Identifier and line.t[2].type == Token::Type::LeftParen)
    {
        if ((line.t.end() - 1)->type == Token::Type::RightParen)
        {
            return true;
        }
        throw ParserExeption("Expected ')'", line.line);
    }
    return false;
}

bool SecondParser::isVariableDec(Lexer::LineWithTokens& line)
{
    if (line.t.size() >= 2 and line.t[0].type == Token::Type::Type and line.t[1].type == Token::Type::Identifier and (line.t.size() == 2 or line.t[2].type != Token::Type::LeftParen))
    {
        return true;
    }
    return false;
}

bool SecondParser::isAssignment(Lexer::LineWithTokens& line)
{
    if (line.t.size() >= 2 and line.t[0].type == Token::Type::Identifier and line.t[1].type == Token::Type::Operator and line.t[1].value == "=")
    {
        if (line.t.size() == 2) throw ParserExeption("Expected expression", line.line);
        return true;
    }
    return false;
}

bool SecondParser::isForLoop(Lexer::LineWithTokens& line)
{
    if (line.t[0].type == Token::Type::Keyword and line.t[0].value == "for")
    {
        int countOfSemicolon = 0;
        for (int i = 1; i < line.t.size(); i++)
        {
            if (line.t[i].type == Token::Type::Semicolon) countOfSemicolon++;
        }
        if (countOfSemicolon != 2) throw ParserExeption("Expected ';'", line.line);
        return true;
    }
    return false;
}

bool SecondParser::isWhileLoop(Lexer::LineWithTokens& line)
{
    if (line.t[0].type == Token::Type::Keyword and line.t[0].value == "while") return true;
    return false;
}

bool SecondParser::isIf(Lexer::LineWithTokens& line)
{
    if (line.t[0].type == Token::Type::Keyword and line.t[0].value == "if") return true;
    return false;
}

VariableDec* SecondParser::findIdentifier(std::string name)
{
    for (auto it = variables.begin(); it != variables.end(); it++)
    {
        if ((*it)->name == name) return *it;
    }
    throw std::runtime_error("Variable " + getBlue(name) + " not declared");
}
