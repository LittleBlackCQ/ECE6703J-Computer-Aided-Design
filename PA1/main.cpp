#include "parser.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    std::string benchfile(argv[1]);
    Parser parser(benchfile);
    std::cout << parser.rt["G1gat"].gate << "; " << parser.rt["G1gat"].outs[0] << std::endl;
    return 0;
}