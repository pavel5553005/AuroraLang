#pragma once

#include "../Types.hpp"

#include "../Parser/Parser.hpp"

class CodeGenerator
{
private:
    struct Data
    {
        std::vector<uint8_t> byteCode;
        std::vector<Parser::DataObject> objects;
    };

    struct Text
    {
        std::vector<uint8_t> byteCode;
        std::vector<Parser::Instruction> instructions;
    };

    struct Syscall
    {
        std::string name;
        uint64_t offset;
    };

    const std::vector<Syscall> syscalls = {
        {"@open", 0x00},
        {"@read", 0x01},
        {"@write", 0x02},
        {"@close", 0x03},
        {"@writeLine", 0x04},
        {"@getSystemTime", 0x05},
    };

    Data data;
    Text text;

    std::vector<Parser::Label> labels;

    uint8_t createRegisters(Parser::Instruction inst);

    template <typename T>
    void paste(T value, std::vector<uint8_t>& code);

    template <typename T>
    void forReplace(std::vector<T>, std::string& arg);
    
    void generateCodeForInstruction(Parser::Instruction& inst);
    void generateCodeForDataObject(Parser::DataObject& obj);

    void replaceImmediate(Parser::Instruction& inst);

    void generateCodeForData();
    void generateCodeForText();
public:
    CodeGenerator();

    void setData(std::vector<Parser::DataObject>& data);
    void setInstructions(std::vector<Parser::Instruction>& instructions);
    void setLabels(std::vector<Parser::Label>& labels);

    std::vector<uint8_t> generateFile();

    std::vector<uint8_t> getDataByteCode() { return data.byteCode; }
    std::vector<uint8_t> getTextByteCode() { return text.byteCode; }

    ~CodeGenerator();
};

CodeGenerator::CodeGenerator()
{
}

CodeGenerator::~CodeGenerator()
{
}
