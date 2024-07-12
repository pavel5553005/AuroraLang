#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../Lexer/Lexer.hpp"
#include "../CompilerExeption.hpp"

class FirstParser
{
public:
    struct Node
    {
        Node* parent;
        std::vector<Node> children;
        Lexer::LineWithTokens line;
    };

    FirstParser(Lexer::Tokanized& tokens) : tokens(tokens) {};

    Node& getRoot() {return root;}

    void parseTokens() {root = parse(tokens);}

    ~FirstParser() { };
private:
    Lexer::Tokanized& tokens;
    Node root;
    Node parse(Lexer::Tokanized& tokens);
    void addNode(Node* parent, Node* child);
};

FirstParser::Node FirstParser::parse(Lexer::Tokanized& tokens)
{
    Node node;
    int cofs = tokens[0].spaceCount;
    for (auto it = tokens.begin();it != tokens.end();)
    {
        if (it->spaceCount < tokens[0].spaceCount) throw ParserExeption("Tabulation error", it->line);
        if (it->spaceCount == tokens[0].spaceCount)
        {
            Node child;
            child.line = *it;
            node.children.push_back(child);
            it++;
            continue;
        }
        if (it->spaceCount > tokens[0].spaceCount)
        {
            cofs = it->spaceCount;
            Lexer::Tokanized tmp;
            while (it != tokens.end() and it->spaceCount >= cofs)
            {
                if (it->spaceCount < cofs and it->spaceCount != tokens[0].spaceCount)
                    throw ParserExeption("Tabulation error", it->line);
                tmp.push_back(*it);
                it++;
            }
            node.children.back().children = parse(tmp).children;
        }
    }
    return node;
}
