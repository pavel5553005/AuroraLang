#pragma once

#include "CodeGenerator.hpp"
#include <iostream>

uint8_t CodeGenerator::createRegisters(Parser::Instruction inst)
{
    uint8_t first = std::stoi(inst.arguments[0].value);
    uint8_t last = std::stoi(inst.arguments[1].value);
    if (first >= 0x08 || last >= 0x08)
        throw std::runtime_error("Invalid rx register");
    return (first << 4) | last;
}

template <typename T>
void CodeGenerator::paste(T value, std::vector<uint8_t>& code)
{
    for (size_t i = 0; i < sizeof(T); ++i) {
        code.push_back(static_cast<uint8_t>(value >> (i * 8)));
    }
}

template <typename T>
void CodeGenerator::forReplace(std::vector<T> values, std::string& arg)
{
    for (auto i : values)
    {
        if (i.name == arg)
        {
            std::cout << "Replacing " << arg << " with " << std::to_string(int(i.offset)) << std::endl;
            arg = std::to_string(int(i.offset));
            return;
        }
    }
    throw std::runtime_error(arg + " is not declared");
}

void CodeGenerator::generateCodeForInstruction(Parser::Instruction& inst)
{
    paste<uint8_t>(inst.opcode, text.byteCode);
    std::string imm;
    switch (inst.opcode)
    {
    case 0x00:
    case 0x01:
        break;
    case 0x02:
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        break;
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x07:
        paste<uint8_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x08:
        break;
    case 0x09:
        paste<uint8_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x0a:
        break;
    case 0x0b:
    case 0x0c:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        paste<uint64_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x0d:
        paste<uint64_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        paste<uint8_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x0e:
    case 0x0f:
        paste<uint64_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x10:
    case 0x11:
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        break;
    case 0x12:
    case 0x13:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x14:
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        paste<uint64_t>(std::stoi(inst.arguments[2].value), text.byteCode);
        break;
    case 0x15:
        imm = inst.arguments[1].value;
        inst.arguments[1].value = inst.arguments[2].value;
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        paste<uint64_t>(std::stoi(imm), text.byteCode);
        break;
    case 0x16:
    case 0x17:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        paste<uint64_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x18:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x19:
    case 0x1a:
        break;
    case 0x1b:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x1c:
    case 0x1d:
        break;
    case 0x1e:
    case 0x1f:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x20:
    case 0x21:
    case 0x22:
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        break;
    case 0x23:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        break;
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30:
    case 0x31:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x32:
    case 0x33:
        break;
    case 0x34:
        paste<uint8_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x35:
    case 0x36:
        break;
    case 0x37:
        paste<uint64_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x38:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    case 0x39:
        paste<uint8_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        paste<uint64_t>(std::stoi(inst.arguments[1].value), text.byteCode);
        break;
    case 0x3a:
    case 0x3b:
        break;
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
        paste<uint8_t>(createRegisters(inst), text.byteCode);
        break;
    case 0x49:
    case 0x4a:
    case 0x4b:
        paste<uint64_t>(std::stoi(inst.arguments[0].value), text.byteCode);
        break;
    default:
        throw std::runtime_error("Unknown opcode");
    }
}

void CodeGenerator::generateCodeForDataObject(Parser::DataObject& obj)
{
    if (obj.type == Parser::DataObject::Type::Number)
    {
        paste<uint64_t>(std::stoi(obj.data), data.byteCode);
    }
    else
    {
        for (auto i : obj.data)
        {
            paste<uint8_t>(i, data.byteCode);
        }
        paste<uint8_t>(0, data.byteCode);
    }
}

void CodeGenerator::replaceImmediate(Parser::Instruction& inst)
{
    for (auto &&i : inst.arguments)
    {
        if (i.value[0] == '.')
        {
            forReplace<Parser::Label>(labels, i.value);
        }
        else if (i.value[0] == '@')
        {
            forReplace<Syscall>(syscalls, i.value);
        }
        else if (isalpha(i.value[0]))
        {
            forReplace<Parser::DataObject>(data.objects, i.value);
        }
    }
}



void CodeGenerator::generateCodeForData()
{
    for (auto i : data.objects)
    {
        generateCodeForDataObject(i);
    }
    
}

void CodeGenerator::generateCodeForText()
{
    for (auto i : text.instructions)
    {
        replaceImmediate(i);
        generateCodeForInstruction(i);
    }
}

void CodeGenerator::setData(std::vector<Parser::DataObject>& data)
{
    this->data.objects = data;
}


void CodeGenerator::setInstructions(std::vector<Parser::Instruction>& instructions)
{
    this->text.instructions = instructions;
}

void CodeGenerator::setLabels(std::vector<Parser::Label>& labels)
{
    this->labels = labels;
}

std::vector<uint8_t> CodeGenerator::generateFile()
{
    generateCodeForData();
    generateCodeForText();

    std::vector<uint8_t> result;
    paste<uint64_t>(data.byteCode.size() + 8, result);
    for (auto &&i : data.byteCode)
    {
        paste<uint8_t>(i, result);
    }
    for (auto &&i : text.byteCode)
    {
        paste<uint8_t>(i, result);
    }
    return result;
}