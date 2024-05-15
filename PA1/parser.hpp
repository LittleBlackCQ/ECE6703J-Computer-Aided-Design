#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "gate.hpp"

class Parser {
public:
    struct Node {
        GateType gate;
        std::vector<std::string> ins;
        std::vector<std::string> outs;

        Node(GateType g, std::vector<std::string> i, std::vector<std::string> o)
            : gate(g), ins(i), outs(o) {}
    };

    typedef std::unordered_map<std::string, Node> DAGraph;

    DAGraph rt;
    std::string remove_spaces(const std::string& str);
    void parse_bench(const std::string& bench_file);
    void parse_val();

};

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
                if (line.substr(0, left_brkt-1) == "INPUT") {
                    std::string name = line.substr(left_brkt+1, right_brkt-left_brkt-1);
                    rt.insert({name, Node(PRIMARY, std::vector<std::string>(), std::vector<std::string>())});
                }
            } else {
                // write internal gates
                size_t eq = line.find('=');
                size_t start = line.find('(');
                size_t pos = line.find(',');

                std::string name = line.substr(0, eq);

                std::string gate_name = line.substr(eq+1, start-eq-1);
                GateType gate_type;
                if (GateNamesUpper.find(gate_name) != GateNamesUpper.end()) 
                    gate_type = GateNamesUpper[gate_name];
                else 
                    gate_type = GateNamesLower[gate_name];

                std::vector<std::string> ins;
                start += 1;
                while (pos != std::string::npos) {
                    std::string in_name = line.substr(start, pos-start);
                    ins.push_back(in_name);
                    rt[in_name].outs.push_back(name);
                    start = pos + 1;
                    pos = line.find(',');
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