#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include "../Types.hpp"

class Parser
{
public:
    struct Instruction
    {
        struct Argument
        {
            ArgumentType type;
            std::string value;

            bool operator==(const Argument& other) const
            {
                return type == other.type;
            }
        };
        struct Type
        {
            uint8_t opcode;
            std::string name;
            std::vector<ArgumentType> arguments;
            uint64_t size;
        };

        std::string name;
        std::vector<Argument> arguments;
        
        uint8_t opcode;
        uint64_t size;
    };
private:
    const std::vector<Instruction::Type> instructionTypes = {
        {0x00, "nop", {}, 1},
        {0x01, "halt", {}, 1},
        {0x02, "mov", {RegRx, RegRx}, 2},
        {0x03, "mov", {RegRx, RegBr}, 2},
        {0x04, "mov", {RegRx, RegTbr}, 2},
        {0x05, "mov", {RegRx, RegSp}, 2},
        {0x06, "mov", {RegRx, RegBp}, 2},
        {0x07, "mov", {RegBr, RegRx}, 2},
        {0x08, "mov", {RegBr, RegTbr}, 1},
        {0x09, "mov", {RegTbr, RegRx}, 2},
        {0x0a, "mov", {RegTbr, RegBr}, 1},
        {0x0b, "mov", {RegRx, Imm}, 10},
        {0x0c, "mov", {RegRx, AddrImm}, 10},
        {0x0d, "mov", {AddrImm, RegRx}, 10},
        {0x0e, "mov", {AddrImm, RegBr}, 9},
        {0x0f, "mov", {AddrImm, RegTbr}, 9},
        {0x10, "mov", {RegRx, AddrRegRx}, 2},
        {0x11, "mov", {AddrRegRx, RegRx}, 2},
        {0x12, "mov", {AddrRegRx, RegBr}, 2},
        {0x13, "mov", {AddrRegRx, RegTbr}, 2},
        {0x14, "mov", {RegRx, AddrRegRx, Imm}, 10},
        {0x15, "mov", {AddrRegRx, Imm, RegRx}, 10},
        {0x16, "mov", {AddrRegRx, Imm, RegBr}, 10},
        {0x17, "mov", {AddrRegRx, Imm, RegTbr}, 10},
        {0x18, "push", {RegRx}, 2},
        {0x19, "push", {RegBr}, 1},
        {0x1a, "push", {RegTbr}, 1},
        {0x1b, "pop", {RegRx}, 2},
        {0x1c, "pop", {RegBr}, 1},
        {0x1d, "pop", {RegTbr}, 1},
        {0x1e, "alloc", {RegRx}, 2},
        {0x1f, "free", {RegRx}, 2},
        {0x20, "and", {RegRx, RegRx}, 2},
        {0x21, "or", {RegRx, RegRx}, 2},
        {0x22, "xor", {RegRx, RegRx}, 2},
        {0x23, "not", {RegRx}, 2},
        {0x24, "shl", {RegRx, RegRx}, 2},
        {0x25, "shr", {RegRx, RegRx}, 2},
        {0x26, "add", {RegRx, RegRx}, 2},
        {0x27, "sub", {RegRx, RegRx}, 2},
        {0x28, "mul", {RegRx, RegRx}, 2},
        {0x29, "div", {RegRx, RegRx}, 2},
        {0x2a, "gre", {RegRx, RegRx}, 2},
        {0x2b, "les", {RegRx, RegRx}, 2},
        {0x2c, "eq", {RegRx, RegRx}, 2},
        {0x2d, "jmp", {RegRx}, 2},
        {0x2e, "jnz", {RegRx}, 2},
        {0x2f, "jz", {RegRx}, 2},
        {0x30, "syscall", {Imm}, 9},
        {0x31, "mov", {RegRx, RegFbr}, 2},
        {0x32, "mov", {RegBr, RegFbr}, 1},
        {0x33, "mov", {RegTbr, RegFbr}, 1},
        {0x34, "mov", {RegFbr, RegRx}, 2},
        {0x35, "mov", {RegFbr, RegBr}, 1},
        {0x36, "mov", {RegFbr, RegTbr}, 1},
        {0x37, "mov", {AddrImm, RegFbr}, 9},
        {0x38, "mov", {AddrRegRx, RegFbr}, 2},
        {0x39, "mov", {AddrRegRx, Imm, RegFbr}, 10},
        {0x3a, "push", {RegFbr}, 1},
        {0x3b, "pop", {RegFbr}, 1},
        {0x3c, "addf", {RegRx, RegRx}, 2},
        {0x3d, "subf", {RegRx, RegRx}, 2},
        {0x3e, "mulf", {RegRx, RegRx}, 2},
        {0x3f, "divf", {RegRx, RegRx}, 2},
        {0x40, "addd", {RegRx, RegRx}, 2},
        {0x41, "muld", {RegRx, RegRx}, 2},
        {0x42, "divd", {RegRx, RegRx}, 2},
        {0x43, "cvtif", {RegRx, RegRx}, 2},
        {0x44, "cvtid", {RegRx, RegRx}, 2},
        {0x45, "cvtfi", {RegRx, RegRx}, 2},
        {0x46, "cvtfd", {RegRx, RegRx}, 2},
        {0x47, "cvtdi", {RegRx, RegRx}, 2},
        {0x48, "cvtdf", {RegRx, RegRx}, 2},
        {0x49, "jmp", {Imm}, 9},
        {0x4a, "jnz", {Imm}, 9},
        {0x4b, "jz", {Imm}, 9}
    };
public:
    struct DataObject
    {
        enum class Type
        {
            String,
            Number
        };

        Type type;
        std::string name;
        uint64_t size;
        uint64_t offset;
        std::string data;
    };

    struct Label
    {
        std::string name;
        uint64_t offset = 0xFF;
    };
private:
    uint64_t dataObjOffset = 0;
    uint64_t instOffset = 0;

    TokanizedFile& tokens;

    std::vector<DataObject> objects;
    std::vector<Instruction> instructions;
    std::vector<Label> labels;

    void parseData(TokanizedFile tokens);
    void parseText(TokanizedFile tokens);
    void parseLabels(TokanizedFile tokens);

    bool isArgumentsEqual(const std::vector<Instruction::Argument>& inst1, const std::vector<ArgumentType>& inst2);

    bool isVariableExists(const std::string& name);
    bool isLabelExists(const std::string& name);
    bool isInstructionExists(const std::string& name);

    void setTypeToInstruction(Instruction& inst);

    DataObject createDataObject(LineWithTokens tokens);
    Instruction::Argument createArgument(Token tokens);
    void createInstruction(LineWithTokens tokens);
    void createLabel(LineWithTokens tokens);
public:
    Parser(TokanizedFile& tokens);

    void parse();

    std::vector<DataObject>& getObjects() { return objects; }
    std::vector<Instruction>& getInstructions() { return instructions; }
    std::vector<Label>& getLabels() { return labels; }

    ~Parser();
};
