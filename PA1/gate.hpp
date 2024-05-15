#include <string>
#include <unordered_map>

enum GateType {
    PRIMARY, AND, OR, NAND, NOR, XOR, XNOR, NOT, BUF
};

std::unordered_map<const std::string, const GateType> GateNamesUpper = {
    {"AND", AND}, 
    {"OR", OR},
    {"NAND", NAND},
    {"NOR", NOR},
    {"XOR", XOR},
    {"XNOR", XNOR},
    {"NOT", NOT},
    {"BUF", BUF}
};

std::unordered_map<const std::string, const GateType> GateNamesLower = {
    {"and", AND}, 
    {"or", OR},
    {"nand", NAND},
    {"nor", NOR},
    {"xor", XOR},
    {"xnor", XNOR},
    {"not", NOT},
    {"buf", BUF}
};
