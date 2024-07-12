#include "Parser.hpp"

Parser::Instruction::Argument Parser::createArgument(Token token)
{
    switch (token.type)
    {
    case TokenType::Immediate:
        return Instruction::Argument { ArgumentType::Imm, token.value };
    case TokenType::Variable:
        if (isVariableExists(token.value) == false)
        {
            throw std::runtime_error("Variable \033[34m" + token.value + "\033[0m not declared");
        }
        return Instruction::Argument { ArgumentType::Imm, token.value };
    case TokenType::Label:
        if (isLabelExists(token.value) == false)
        {
            throw std::runtime_error("Label \033[34m" + token.value + "\033[0m not declared");
        }
        return Instruction::Argument { ArgumentType::Imm, token.value };
    case TokenType::Register:
        if (token.value[0] == 'r')
        {
            if (!(token.value[1] >= '0' && token.value[1] < '8'))
            {
                throw std::runtime_error("Invalid register\033[34mr rx\033[0m definition");
            }
            return Instruction::Argument { ArgumentType::RegRx, std::to_string(token.value[1] - '0') };
        }
        else if (token.value == "br")
            return Instruction::Argument { ArgumentType::RegBr, "0" };
        else if (token.value == "tbr")
            return Instruction::Argument { ArgumentType::RegTbr, "0" };
        else if (token.value == "fbr")
            return Instruction::Argument { ArgumentType::RegFbr, "0" };
        else if (token.value == "sp")
            return Instruction::Argument { ArgumentType::RegSp, "0" };
        else if (token.value == "bp")
            return Instruction::Argument { ArgumentType::RegBp, "0" };
        else
            throw std::runtime_error("Invalid register definition");
    case TokenType::Syscall:
        return Instruction::Argument { ArgumentType::Imm, token.value };
    case TokenType::AddressRegister:
        if (token.value[0] == 'r' && token.value.size() == 2 && isdigit(token.value[1]) && token.value[1] >= '0' && token.value[1] < '8')
            return Instruction::Argument { ArgumentType::AddrRegRx, std::to_string(token.value[1] - '0') };
        else
            throw std::runtime_error("Address register can be only\033[34mr rx\033[0m register");
    case TokenType::AddressImmediate:
        if (token.value[0] == '.')
        {
            if (isLabelExists(token.value) == false)
                throw std::runtime_error("Label \033[34m" + token.value + "\033[0m not declared");
            return Instruction::Argument { ArgumentType::AddrImm, token.value };
        }
        else if (isdigit(token.value[0]))
            return Instruction::Argument { ArgumentType::AddrImm, token.value };
        else if (isVariableExists(token.value) == false)
            throw std::runtime_error("Variable \033[34m" + token.value + "\033[0m not declared");
        else
            return Instruction::Argument { ArgumentType::AddrImm, token.value };
    default:
        throw std::runtime_error("Invalid argument type");
    }
}

void Parser::createInstruction(LineWithTokens tokens)
{
    if (tokens.tokens[0].type == TokenType::Label)
    {
        for (auto &&label : labels)
        {
            if (label.name == tokens.tokens[0].value)
            {
                label.offset = instOffset;
                return;
            }
        }
        throw std::runtime_error("Internal error: label \033[34m" + tokens.tokens[0].value + "\033[0m not found");
    }
    try
    {
        if (tokens.tokens[0].type != TokenType::Instruction) throw std::runtime_error("\033[34m" + tokens.tokens[0].value + "\033[0m is not an instruction");
        if (tokens.tokens.size() < 2 && tokens.tokens[0].value != "nop" && tokens.tokens[0].value != "halt") throw std::runtime_error("Too few arguments");
        if (tokens.tokens.size() > 7) throw std::runtime_error("Too many arguments");
        if (tokens.tokens.size() >= 3 && tokens.tokens[2].type != TokenType::Comma || tokens.tokens.size() >= 5 && tokens.tokens[4].type != TokenType::Comma || tokens.tokens.size() >= 7 && tokens.tokens[6].type != TokenType::Comma)
            throw std::runtime_error("Expected comma");
        if (tokens.tokens.size() % 2 != 0 && tokens.tokens[0].value != "nop" && tokens.tokens[0].value != "halt") throw std::runtime_error("Expected argument");
    }
    catch(const std::exception& e)
    {
        throw CompilerException(std::string("Invalid instruction definition: ") + e.what(), tokens.line);
    }
    
    

    Parser::Instruction inst;
    inst.name = tokens.tokens[0].value;

    try
    {
        for (int i = 1; i < tokens.tokens.size(); i += 2)
        {
            inst.arguments.push_back(createArgument(tokens.tokens[i]));
        }

        setTypeToInstruction(inst);
    }
    catch(const std::exception& e)
    {
        throw CompilerException(std::string("Invalid instruction definition: ") + e.what(), tokens.line);
    }

    instructions.push_back(inst);
    instOffset += inst.size;
}

void Parser::createLabel(LineWithTokens tokens)
{
    if (tokens.tokens[0].type == TokenType::Label)
    {
        Label label;
        label.name = tokens.tokens[0].value;
    
        for (auto label : labels)
        {
            if (label.name == tokens.tokens[0].value)
            {
                throw CompilerException("Label \033[34m" + tokens.tokens[0].value + "\033[0m already declared", tokens.line);
            }
        }
        labels.push_back(label);
    }
}

Parser::DataObject Parser::createDataObject(LineWithTokens tokens)
{
    if (tokens.tokens.size() != 3 || tokens.tokens[0].type != TokenType::Var || tokens.tokens[1].type != TokenType::Variable || (tokens.tokens[2].type != TokenType::Immediate && tokens.tokens[2].type != TokenType::String))
        throw CompilerException("Invalid data object definition", tokens.line);


    for (auto i : objects)
    {
        if (i.name == tokens.tokens[1].value)
            throw CompilerException("Data object \033[34m" + tokens.tokens[1].value + "\033[0m already declared", tokens.line);
    }
    

    DataObject obj;
    if (tokens.tokens[2].type == TokenType::Immediate)
    {
        obj.type = DataObject::Type::Number;
        obj.size = 8;
    }
    else
    {
        obj.type = DataObject::Type::String;
        obj.size = tokens.tokens[2].value.size() + 1;
    }
    obj.data = tokens.tokens[2].value;
    obj.name = tokens.tokens[1].value;
    obj.offset = dataObjOffset;

    dataObjOffset += obj.size;

    return obj;
}
