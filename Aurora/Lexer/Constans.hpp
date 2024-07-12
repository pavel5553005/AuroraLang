#pragma once

#include <string>
#include <vector>

const std::vector<std::string> Keywords = {
    "for",
    "while",
    "do",

    "true",
    "false",

    "if",
    "else",

    "switch",
    "case",
    "default",

    "break",
    "continue",
    "return",
    "import",

    "class",
    "enum",
    "interface",

    "try",
    "catch",
    "finally",
    "throw",
    "throws",

    "const",
    "static",

    "private",
    "protected",
    "public",

    "abstract"

    "and",
    "or"
};

const std::vector<std::string> Types = {
    "void",
    "bool",
    "char",
    "short",
    "int",
    "long",
    "float",
    "double",
};

const std::vector<std::string> Operators = {
    "!",

    "==",
    "!=",
    ">",
    "<",
    ">=",
    "<=",

    "+",
    "-",

    "*",
    "/",
    "%",

    "++",
    "--",

    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
};

const std::string red = "\033[1;31m";
const std::string green = "\033[1;32m";
const std::string blue = "\033[1;34m";
const std::string yellow = "\033[1;33m";
const std::string cyan = "\033[1;36m";
const std::string magenta = "\033[1;35m";
const std::string reset = "\033[0m";

std::string getRed(std::string s) { return red + s + reset; }
std::string getGreen(std::string s) { return green + s + reset; }
std::string getBlue(std::string s) { return blue + s + reset; }
std::string getYellow(std::string s) { return yellow + s + reset; }
std::string getCyan(std::string s) { return cyan + s + reset; }
std::string getMagenta(std::string s) { return magenta + s + reset; }
