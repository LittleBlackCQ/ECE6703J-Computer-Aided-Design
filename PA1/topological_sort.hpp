#include <deque>
#include <iostream>
#include "parser.hpp"
#include "gate.hpp"

#pragma once

class TopolocigalSort {
public:
    TopolocigalSort() : queue(std::deque<Parser::Node>()) {};
    void calculate_output(Parser::DAGraph &);

private:
    std::deque<Parser::Node> queue;
    bool gate_return(Gate::GateType, bool, bool);
};

bool TopolocigalSort::gate_return(Gate::GateType type, bool a, bool b) {
    if (type == Gate::AND || type == Gate::NAND) 
        return a&b;
    if (type == Gate::OR || type == Gate::NOR) 
        return a|b;
    if (type == Gate::XOR || type == Gate::XNOR) 
        return (!a&b)|(a&!b);
    return true;
}

void TopolocigalSort::calculate_output(Parser::DAGraph &graph) {
    // init number of ins and queue
    for (auto &[name, node] : graph) {
        node.num_ins = node.ins.size();
        if (node.num_ins == 0) queue.push_back(node);
    }

    // tpological sort
    while (!queue.empty()) {
        Parser::Node &node = queue.front();
        for (auto out : node.outs) {
            Parser::Node &out_node = graph[out];

            out_node.tmp_val = out_node.num_ins == out_node.ins.size() ? 
            node.tmp_val : gate_return(out_node.gate, out_node.tmp_val, node.tmp_val);
            if (out_node.num_ins == 1 && (out_node.gate == Gate::NAND || out_node.gate == Gate::NOR || out_node.gate == Gate::XNOR || out_node.gate == Gate::NOT))
                out_node.tmp_val = !out_node.tmp_val;
            out_node.num_ins -= 1;

            if (out_node.num_ins == 0) queue.push_back(out_node);
        } 
        queue.pop_front();
    }
}