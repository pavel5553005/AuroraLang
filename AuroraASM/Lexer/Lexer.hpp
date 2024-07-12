#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <stdint.h>

#include "../Types.hpp"

class Lexer
{
private:
    const std::vector<std::string> instructions = {
        "nop",
        "halt",
        "mov",
        "push",
        "pop",
        "alloc",
        "free",
        "and",
        "or",
        "xor",
        "not",
        "shl",
        "shr",
        "add",
        "sub",
        "mul",
        "div",
        "gre",
        "les",
        "eq",
        "jmp",
        "jnz",
        "jz",
        "syscall",
        "addf",
        "subf",
        "mulf",
        "divf",
        "addd",
        "subd",
        "muld",
        "divd",
        "cvtif",
        "cvtid",
        "cvtfi",
        "cvtfd",
        "cvtdi",
        "cvtdf"
    };
    const std::vector<std::string> registers = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "br", "tbr", "sp", "bp"};

    std::string source;
    TokanizedFile tokens;

    std::vector<Token> tokenizeLine(std::string line);
    void skipSpaces(std::string line);

    int index = 0;

    Token string(std::string line);
    Token label(std::string line);
    Token syscall(std::string line);
    std::vector<Token> address(std::string line);
    Token word(std::string line);
public:
    Lexer(std::string source);
    TokanizedFile tokenize();
};
