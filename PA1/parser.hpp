#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "gate.hpp"

#pragma once

class Parser {
public:
    struct Node {
        Gate::GateType gate;
        std::vector<std::string> ins;
        std::vector<std::string> outs;

        bool tmp_val = false;
        unsigned num_ins = 0;

        Node() : gate(Gate::PRIMARY), ins(std::vector<std::string>()), outs(std::vector<std::string>()) {};
        Node(Gate::GateType g, std::vector<std::string> i, std::vector<std::string> o)
            : gate(g), ins(i), outs(o) {}
    };

    typedef std::unordered_map<std::string, Node> DAGraph;

    DAGraph rt;
    Parser() = delete;
    Parser(const std::string&, const std::string&);

    void print_output();

private:
    std::vector<std::string> output_queue;
    std::string remove_spaces(const std::string&);
    void parse_bench(const std::string&);
    void parse_val(const std::string&);
};

Parser::Parser(const std::string& bench_file, const std::string& val_file) {
    parse_bench(bench_file);
    parse_val(val_file);
}

std::string Parser::remove_spaces(const std::string& str) {
    std::string rt;
    size_t pos = str.find(' ');
    size_t start = 0;

    while (pos != std::string::npos) {
        rt += str.substr(start, pos - start);
        start = pos + 1;
        pos = str.find(' ', start);
    }
    rt += str.substr(start);
    return rt;
}

void Parser::parse_bench(const std::string& bench_file) {

    std::ifstream ifs(bench_file, std::ios::in);
    if (ifs.is_open()) {
        std::string line;
        while (std::getline(ifs, line)) {
            line = remove_spaces(line);
            if (line.length() == 0 || line[0] == '#') continue;
            if (line.find('=') == std::string::npos) {
                // write input and output gates
                size_t left_brkt = line.find('(');
                size_t right_brkt = line.find(')');
                if (line.substr(0, left_brkt) == "INPUT") {
                    std::string name = line.substr(left_brkt+1, right_brkt-left_brkt-1);
                    rt.insert({name, Node(Gate::PRIMARY, std::vector<std::string>(), std::vector<std::string>())});
                } else if (line.substr(0, left_brkt) == "OUTPUT") {
                    output_queue.push_back(line.substr(left_brkt+1, right_brkt-left_brkt-1));
                }
            } else {
                // write internal gates
                size_t eq = line.find('=');
                size_t start = line.find('(');
                size_t pos = line.find(',');

                std::string name = line.substr(0, eq);

                std::string gate_name = line.substr(eq+1, start-eq-1);
                Gate::GateType gate_type;
                if (Gate::GateNamesUpper.find(gate_name) != Gate::GateNamesUpper.end()) 
                    gate_type = Gate::GateNamesUpper[gate_name];
                else 
                    gate_type = Gate::GateNamesLower[gate_name];

                std::vector<std::string> ins;
                start += 1;
                while (pos != std::string::npos) {
                    std::string in_name = line.substr(start, pos-start);
                    ins.push_back(in_name);
                    rt[in_name].outs.push_back(name);
                    start = pos + 1;
                    pos = line.find(',', start);
                }
                pos = line.find(')');
                std::string in_name = line.substr(start, pos-start);
                ins.push_back(in_name);
                rt[in_name].outs.push_back(name);
                rt.insert({name, Node(gate_type, ins, std::vector<std::string>())});
            }
        }
    }
}

void Parser::parse_val(const std::string& val_file) {

    std::ifstream ifs(val_file, std::ios::in);
    if (ifs.is_open()) {
        std::string line;
        while (std::getline(ifs, line)) {
            size_t space = line.find(' ');
            std::string name = line.substr(0, space);
            rt[name].tmp_val = line.substr(space+1) == "0" ? false : true;
        }
    }
}

void Parser::print_output() {
    for (auto output : output_queue) {
        std::cout << output << " " << rt[output].tmp_val << std::endl;
    }
}