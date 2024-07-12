#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../CompilerExeption.hpp"

class Node
{
private:
public:
    std::vector<Node*> children;
    Node* parent;
    int line;
    Node();
    void addChild(Node* child);
    void addChildren(std::vector<Node*> children);
    virtual ~Node();
};

Node::Node()
{
}

void Node::addChild(Node* child)
{
    children.push_back(child);
    child->parent = this;
}

void Node::addChildren(std::vector<Node*> children)
{
    for (auto it = children.begin(); it != children.end(); it++)
    {
        this->addChild(*it);
    }
}

Node::~Node()
{
}
