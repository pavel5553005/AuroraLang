#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Lexer/Lexer.cpp"
#include "Parser/Parser.cpp"
#include "Parser/Creators.cpp"
#include "CodeGenerator/CodeGenerator.cpp"

void printError(CompilerException e, std::vector<std::string> lines);

void printInstruction(Parser::Instruction inst);

void printHex(uint8_t value);

void writeFile(std::string name, std::vector<uint8_t> byteCode);

int main(int argc, char** argv)
{
    // if (argc != 2)
    // {
    //     std::cerr << "Usage: aurasm [filename].aurasm" << std::endl;
    //     return 1;
    // }

    std::ifstream file("test.aurasm");
    if (!file)
    {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }
    std::string scr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::istringstream ss(scr);
    file.close();
    TokanizedFile tokens;

    std::vector<std::string> lines;

    for (std::string line; std::getline(ss, line); )
    {
        lines.push_back(line);
    }

    std::cout << "\033[32mCompiling...\033[0m" << std::endl;

    std::cout << "Lexing..." << std::endl;

    Lexer lexer(scr);
    try
    {
        tokens = lexer.tokenize();
    }
    catch(const CompilerException& e)
    {
        printError(e, lines);
        return 1;
    }

    Parser parser(tokens);
    std::cout << "Parsing..." << std::endl;

    try
    {
        parser.parse();
    }
    catch(const CompilerException& e)
    {
        printError(e, lines);
        return 1;
    }

    for (auto inst : parser.getInstructions())
    {
        printInstruction(inst);
    }

    std::cout << "Code generation..." << std::endl;

    CodeGenerator codeGen;

    codeGen.setInstructions(parser.getInstructions());
    codeGen.setData(parser.getObjects());
    codeGen.setLabels(parser.getLabels());

    std::vector<uint8_t> byteCode;
    try
    {
        byteCode = codeGen.generateFile();
    }
    catch(const std::runtime_error& e)
    {
        printError(CompilerException(e.what(), 0), lines);
        return 1;
    }
    byteCode.push_back(0x00);

    writeFile("program.aurbc", byteCode);

    std::cout << "\033[32m\nSuccessfully compiled!\033[0m" << std::endl;
}

void printError(CompilerException e, std::vector<std::string> lines)
{
    static std::string red = "\033[31m";
    static std::string reset = "\033[0m";

    std::cerr << red << "Error ocurred\033[0m at line " << e.line << ": " << e.what() << "\t\t"<< red << lines[e.line - 1] << reset << std::endl << std::endl;
}

void printInstruction(Parser::Instruction inst)
{
    printHex(inst.opcode);
    std::cout << "\t\033[34m" << inst.name << "\t\t\033[0m";
    for (auto arg : inst.arguments)
    {
        std::cout << " " << arg.value;
    }
    std::cout << std::endl;
}

void printHex(uint8_t value)
{
    std::stringstream ss;
    ss << std::hex << int(value);
    std::cout << "0x";
    if(ss.str().size() == 1)
        std::cout << "0";
    std::cout << ss.str();
}

void writeFile(std::string name, std::vector<uint8_t> byteCode)
{
    std::ofstream file(name, std::ios::binary);
    file.write((char*)&byteCode[0], byteCode.size());
    file.close();
}
