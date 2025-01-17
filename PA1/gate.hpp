#include <string>
#include <unordered_map>

#pragma once

class Gate {
public:
    enum GateType {
        PRIMARY, AND, OR, NAND, NOR, XOR, XNOR, NOT, BUF
    };

    static std::unordered_map<std::string, GateType> GateNamesUpper;

    static std::unordered_map<std::string, GateType> GateNamesLower;
};

std::unordered_map<std::string, Gate::GateType> Gate::GateNamesUpper = {
    {"AND", AND}, 
    {"OR", OR},
    {"NAND", NAND},
    {"NOR", NOR},
    {"XOR", XOR},
    {"XNOR", XNOR},
    {"NOT", NOT},
    {"BUF", BUF}
};

std::unordered_map<std::string, Gate::GateType> Gate::GateNamesLower = {
    {"and", AND}, 
    {"or", OR},
    {"nand", NAND},
    {"nor", NOR},
    {"xor", XOR},
    {"xnor", XNOR},
    {"not", NOT},
    {"buf", BUF}
};