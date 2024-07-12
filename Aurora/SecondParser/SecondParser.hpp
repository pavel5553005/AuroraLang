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

class SecondParser
{
private:
    FirstParser::Node& nodes;
    Expression* root;
    std::vector<VariableDec*> variables;


    Expression::Type getTypeFromToken(Token t);
    Expression::Type getTypeFromString(std::string s);

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

Expression::Type SecondParser::getTypeFromToken(Token t)
{
    if (t.type == Token::Type::Type)
    {
        if (t.value == "bool")
        {
            return Expression::Type::Bool;
        }
        else if (t.value == "char")
        {
            return Expression::Type::Char;
        }
        else if (t.value == "short")
        {
            return Expression::Type::Short;
        }
        else if (t.value == "int")
        {
            return Expression::Type::Int;
        }
        else if (t.value == "long")
        {
            return Expression::Type::Long;
        }
        else if (t.value == "float")
        {
            return Expression::Type::Float;
        }
        else if (t.value == "double")
        {
            return Expression::Type::Double;
        }
    }
    return Expression::Type::Void;
}

Expression::Type SecondParser::getTypeFromString(std::string s)
{
    auto isInteger = [](const std::string& s) -> bool {
        for (char c : s) {
            if (!std::isdigit(c) && c != '+' && c != '-') {
                return false;
            }
        }
        return true;
    };

    // Проверяем, является ли строка double
    auto isDouble = [](const std::string& s) -> bool {
        bool pointSeen = false;
        for (char c : s) {
            if (!std::isdigit(c) && c != 'e' && c != 'E' && c != '+' && c != '-' && c != '.') {
                return false;
            }
            if (c == '.') {
                if (pointSeen) return false;
                pointSeen = true;
            }
        }
        return pointSeen;
    };

    // Проверяем, является ли строка float
    auto isFloat = [isDouble](const std::string& s) -> bool {
        if (s.back() != 'f' && s.back() != 'F') return false;
        return isDouble(s.substr(0, s.size() - 1));
    };

    // Проверяем, является ли строка bool
    auto isBool = [](const std::string& s) -> bool {
        return s == "true" || s == "false";
    };

    if (isBool(s)) return Expression::Type::Bool;
    if (isInteger(s)) return Expression::Type::Int;
    if (isDouble(s)) return Expression::Type::Double;
    if (isFloat(s)) return Expression::Type::Float;
    throw ParserExeption("Unknown type", -1);
}
