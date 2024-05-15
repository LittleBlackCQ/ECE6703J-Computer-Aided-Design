#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gate.h"

class Parser {
public:
    struct Node {
        std::string name;
        GateType gate;
        std::vector<std::string> outs;
    };

    typedef std::vector<Node> DAGraph;

    std::string remove_spaces(const std::string& str);
    DAGraph parse_bench(const std::string& bench_file);
    DAGraph parse_val();

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

Parser::DAGraph Parser::parse_bench(const std::string& bench_file) {
    DAGraph rt;

    std::ifstream ifs(bench_file, std::ios::in);
    if (ifs.is_open()) {
        std::string line;
        while (std::getline(ifs, line)) {
            line = remove_spaces(line);
            if (line.length() == 0 || line[0] == '#') continue;
            if (line.find('=') == std::string::npos) {
                // write input output gates
                size_t left_brkt = line.find('(');
                size_t right_brkt = line.find(')');
                if (line.substr(0, left_brkt-1) == "INPUT")
                    rt.push_back(Node());
            } else {
                // write internal gates
            }
        }
    }
}