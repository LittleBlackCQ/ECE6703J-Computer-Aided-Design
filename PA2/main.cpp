#include "cubeList.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <inputfile>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Cannot open input file " << filename << std::endl;
        return 1;
    }

    cubeList cl;
    inputFile >> cl;
    inputFile.close();

    std::cout << !cl;

    return 0;
}