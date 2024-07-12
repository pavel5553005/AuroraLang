#include "Parser.hpp"

Parser::Parser(TokanizedFile& tokens) : tokens(tokens) { }

void Parser::parse()
{
    if (tokens[0].tokens[0].type != TokenType::Segment || tokens[0].tokens[1].type != TokenType::Variable || tokens[0].tokens[1].value != "data")
        throw CompilerException("Invalid file format", 0);
    
    TokanizedFile data;
    auto it = tokens.begin() + 1;
    for (; it != tokens.end(); it++)
    {
        if (it->tokens[0].type == TokenType::Segment)
            break;
        data.push_back(*it);
    }

    parseData(data);

    if (it->tokens[0].type != TokenType::Segment || it->tokens[1].type != TokenType::Variable || it->tokens[1].value != "text")
        throw CompilerException("Invalid file format", it->line);

    it++;
    
    tokens.erase(tokens.begin(), it);
    parseLabels(tokens);
    parseText(tokens);
}

void Parser::parseData(TokanizedFile tokens)
{
    for (auto line : tokens)
    {
        DataObject obj = createDataObject(line);
        objects.push_back(obj);
    }
}

void Parser::parseText(TokanizedFile tokens)
{
    for (auto line : tokens)
    {
        createInstruction(line);
    }
    if (instructions.size() < 1)
        throw CompilerException("No instructions", 0);
    if (instructions[instructions.size() - 1].name != "halt")
        createInstruction(LineWithTokens {{{TokenType::Instruction, "halt"}}, 9999});
}

void Parser::parseLabels(TokanizedFile tokens)
{
    for (auto line : tokens)
    {
        createLabel(line);
    }
}

void Parser::setTypeToInstruction(Instruction& inst)
{
    if (isInstructionExists(inst.name) == false)
        throw std::runtime_error("Unknown instruction \033[34m" + inst.name + "\033[0m");
    for (auto i : instructionTypes)
    {
        if (i.name == inst.name && isArgumentsEqual(inst.arguments, i.arguments))
        {
            inst.opcode = i.opcode;
            inst.size = i.size;
            return;
        }
    }
    throw std::runtime_error("Incorrect use of instruction \033[34m" + inst.name + "\033[0m");
}

bool Parser::isArgumentsEqual(const std::vector<Parser::Instruction::Argument>& inst1, const std::vector<ArgumentType>& inst2)
{
    if (inst1.size() != inst2.size())
        return false;
    for (int i = 0; i < inst1.size(); i++)
    {
        if (inst1[i].type != inst2[i])
            return false;
    }
    return true;
}

bool Parser::isInstructionExists(const std::string& value)
{
    for (auto i : instructionTypes)
    {
        if (i.name == value)
            return true;
    }
    return false;
}

bool Parser::isLabelExists(const std::string& value)
{
    for (auto i : labels)
    {
        if (i.name == value)
            return true;
    }
    return false;
}

bool Parser::isVariableExists(const std::string& value)
{
    for (auto i : objects)
    {
        if (i.name == value)
            return true;
    }
    return false;
}

Parser::~Parser() { }
