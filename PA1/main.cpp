#include "parser.hpp"
#include "topological_sort.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 3) return 0;

    std::string benchfile(argv[1]);
    std::string valfile(argv[2]);

    Parser parser(benchfile, valfile);
    TopolocigalSort sort;
    sort.calculate_output(parser.rt);
    
    parser.print_output();
    return 0;
}