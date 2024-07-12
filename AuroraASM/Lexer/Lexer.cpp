#include "Lexer.hpp"

Lexer::Lexer(std::string source) : source(source) { }

void Lexer::skipSpaces(std::string line)
{
    while (isspace(line[index]))
        index++;
}

TokanizedFile Lexer::tokenize()
{
    std::stringstream ss(source);

    std::string line;
    int c = 0;

    while (std::getline(ss, line))
    {
        c++;
        try
        {
            std::vector<Token> t = tokenizeLine(line);
            if (t.size() > 0)
                tokens.push_back(LineWithTokens { t, c });
        }
        catch(std::exception& e)
        {
            CompilerException ex(e.what(), c);
            throw ex;
        }
        index = 0;
    }

    return tokens;
}

std::vector<Token> Lexer::tokenizeLine(std::string line)
{
    if (line.size() == 0)
        return std::vector<Token>();

    std::vector<Token> tokens;

    while (index < line.size())
    {
        char c = line[index];
        if (isspace(c))
        {
            index++;
            continue;
        }
        if (c == ',')
        {
            tokens.push_back(Token { TokenType::Comma, "," });
            index++;
            continue;
        }
        if (c == '"')
        {
            tokens.push_back(string(line));
            continue;
        }
        if (c == '#')
        {
            return tokens;
        }
        if (c == '[')
        {
            for (auto i : address(line))
            {
                tokens.push_back(i);
            }
            continue;
        }
        if (isdigit(c) || c == '_' || isalpha(c) || c == '.' || c == '@')
        {
            tokens.push_back(word(line));
            continue;
        }

        throw std::runtime_error("Unknown token at " + std::to_string(index) + ": " + std::string(1, c));
    }
    index = 0;
    return tokens;
}

Token Lexer::word(std::string line)
{
    std::string value = "";
    value.append(1, line[index]);
    index++;
    if (value == "0" && (line[index] == 'x' || line[index] == 'X'))
    {
        value.append(1, line[index]);
        index++;
        while (isxdigit(line[index]))
        {
            value += line[index];
            index++;
        }
        try
        {
            uint64_t num = std::stoull(value, nullptr, 16);
            return Token { TokenType::Immediate, std::to_string(num) };
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("Invalid immediate");
        }
    }
    if (value == "0" && (line[index] == 'b' || line[index] == 'B'))
    {
        value.append(1, line[index]);
        index++;
        while (line[index] == '0' || line[index] == '1')
        {
            value += line[index];
            index++;
        }
        try
        {
            uint64_t num = std::stoull(value, nullptr, 2);
            return Token { TokenType::Immediate, std::to_string(num) };
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("Invalid immediate");
        }
    }
    if (isdigit(value[0]))
    {
        while (isdigit(line[index]))
        {
            value += line[index];
            index++;
        }
    }
    if (value == ".")
    {
        index--;
        return label(line);
    }
    if (value == "@")
    {
        index--;
        return syscall(line);
    }
    
    if (isalpha(value[0]))
    {
        while (isalpha(line[index]) || isdigit(line[index]) || line[index] == '_')
        {
            value += line[index];
            index++;
        }
        if (value == "segment")
            return Token { TokenType::Segment, value };
        if (value == "var")
            return Token { TokenType::Var, value };
        for (auto i : instructions)
        {
            if (value == i)
                return Token { TokenType::Instruction, value };
        }
        for (auto i : registers)
        {
            if (value == i)
                return Token { TokenType::Register, value };
        }
        return Token { TokenType::Variable, value };
    }
    try
    {
        uint64_t num = std::stoull(value, nullptr, 10);
        return Token { TokenType::Immediate, std::to_string(num) };
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Invalid immediate");
    }
    

}

Token Lexer::string(std::string line)
{
    std::string value = "";
    index++;
    while (line[index] != '"')
    {
        if (index == line.size())
            throw std::runtime_error("Unterminated string");
        value += line[index];
        index++;
    }

    int pos = value.find('\\');
    while ((pos = value.find('\\')) != std::string::npos && value[pos + 1] != '"')
    {
        value.replace(pos, 2, "\n");
    }
    index++;
    return Token { TokenType::String, value };
}

Token Lexer::label(std::string line)
{
    std::string value = "";
    while (isalpha(line[index]) || isdigit(line[index]) || line[index] == '_' || line[index] == '.')
    {
        value += line[index];
        index++;
    }
    index++;
    return Token { TokenType::Label, value };
}

Token Lexer::syscall(std::string line)
{
    std::string value = "@";
    index++;
    while (isalpha(line[index]))
    {
        value += line[index];
        index++;
    }
        index++;
    return Token { TokenType::Syscall, value };
}

std::vector<Token> Lexer::address(std::string line)
{
    bool isReg = false;

    std::vector<Token> tokens;
    std::string value = "";
    index++;
    skipSpaces(line);

    int idx = index;
    while (line[idx] != ']' && line[idx] != '+' && line[idx] != '-' && isspace(line[idx]) == 0)
    {
        if (idx == line.size())
            throw std::runtime_error("Unterminated address");
        value += line[idx];
        idx++;
    }

    for (auto i : registers)
    {
        if (value == i)
        {
            isReg = true;
            tokens.push_back(Token { TokenType::AddressRegister, value });
            index = idx;
            break;
        }
    }
    if (!isReg)
    {
        tokens.push_back(Token { TokenType::AddressImmediate, word(line).value });
    }

    skipSpaces(line);

    if (line[index] == '+')
    {
        tokens.push_back(Token { TokenType::Plus, "+" });
        index++;
        skipSpaces(line);
        tokens.push_back(Token { TokenType::AddressImmediate, word(line).value });
        skipSpaces(line);
    }
    if (line[index] == '-')
    {
        throw std::runtime_error("Negative addresses not supported");
        // tokens.push_back(Token { TokenType::Minus, "-" });
        // index++;
        // skipSpaces(line);
        // tokens.push_back(Token { TokenType::AddressImmediate, word(line).value });
        // skipSpaces(line);
    }

    if (tokens.size() == 3)
    {
        if (tokens[0].type == TokenType::AddressImmediate && tokens[1].type == TokenType::Plus && tokens[2].type == TokenType::AddressImmediate)
        {
            uint64_t num = std::stoull(tokens[0].value, nullptr, 10) + std::stoull(tokens[2].value, nullptr, 10);
            tokens.clear();
            tokens.push_back(Token { TokenType::AddressImmediate, std::to_string(num) });
        }
        else if (tokens[0].type == TokenType::AddressImmediate && tokens[1].type == TokenType::Minus && tokens[2].type == TokenType::AddressImmediate)
        {
            uint64_t num = std::stoull(tokens[0].value, nullptr, 10) - std::stoull(tokens[2].value, nullptr, 10);
            tokens.clear();
            tokens.push_back(Token { TokenType::AddressImmediate, std::to_string(num) });
        }
    }
    

    index++;
    return tokens;
}
