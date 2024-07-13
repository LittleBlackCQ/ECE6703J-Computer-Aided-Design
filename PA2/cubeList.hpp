#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma once

class cubeList {
public:
    enum literal {
        Dontcare, Positive, Negative
    };
    typedef std::vector<literal> cube;
    typedef std::vector<cube> cubelist;
    typedef std::pair<size_t, literal> input;

    // Constructor with initial values
    cubeList(int numLiterals, const std::vector<cube>& cubelist)
        : numLiterals(numLiterals), cl(cubelist) {}
    cubeList(int numLiterals)
        : numLiterals(numLiterals) {}

    // Default constructor
    cubeList() : numLiterals(0) {}

    // whether the cubelist is empty
    bool empty() const {
        return cl.empty();
    }

    // get the complement of current cubelist
    cubeList operator!() const; 

    // get the most binate variable
    size_t select_most_binate() const;

    // output a cubelist
    friend std::ostream& operator<<(std::ostream& os, const cubeList& cl); 

    // parse file
    friend std::istream& operator>>(std::istream& is, cubeList& cl);

    // cofactor of a literal and a cubelist
    friend cubeList operator>(input in, const cubeList& cl);

    // and of a literal and a cubelist
    friend cubeList operator*(input in, const cubeList& cl);

    // or of a cubelist and a cubelist
    friend cubeList operator+(const cubeList& cl1, const cubeList& cl2);

private:
    size_t numLiterals;
    cubelist cl; 
};

cubeList cubeList::operator!() const {
    // empty case
    if (cl.empty())
        return cubeList(numLiterals, cubelist(1, cube(numLiterals, Dontcare)));
    
    // only one cube case
    if (cl.size() == 1) {
        cubeList temp(numLiterals, cubelist(1, cube(numLiterals, Dontcare)));
        cubeList rt(numLiterals);
        for (size_t t = 0; t < cl[0].size(); t++) {
            literal l = cl[0][t];
            if (l == Negative) rt = rt + (input(t, Positive) * temp);
            else if (l == Positive) rt = rt + (input(t, Negative) * temp);
            else continue;
        }
        return rt;
    }

    // has all dontcare case
    for (auto const& cube : cl) {
        bool all_dontcares = true;
        for (auto const& l : cube)
            if (l != Dontcare) all_dontcares = false;
        if (all_dontcares) return cubeList(numLiterals);
    }

    size_t x = select_most_binate();
    cubeList P = !(input(x, Positive) > (*this));
    cubeList N = !(input(x, Negative) > (*this));
    P = input(x, Positive) * P;
    N = input(x, Negative) * N;
    return P + N;
}

size_t cubeList::select_most_binate() const {
    int most_unate = -1;
    int most_binate = -1;
    int min_unbalance = INT32_MAX;
    size_t rt = 0;
    for (size_t i = 0; i < numLiterals; i++) {
        bool is_unate = true;
        literal first = Dontcare;
        int binate_size = 0;
        int unblance_size = 0;

        for (const auto& cube : cl) {
            if ((cube[i] != Dontcare)){
                if (first == Dontcare) first = cube[i];
                else if (cube[i] != first) is_unate = false;
                binate_size += 1;
                unblance_size += (cube[i] == Positive) ? 1 : -1;
            }
        }
        unblance_size = abs(unblance_size);
        if (!is_unate && (binate_size > most_binate || (binate_size == most_binate && unblance_size < min_unbalance))) {
            most_binate = binate_size;
            min_unbalance = unblance_size;
            rt = i;
        } else if (is_unate && most_binate == -1 && binate_size > most_unate) {
            most_unate = binate_size;
            rt = i;
        }
    }
    return rt;
}

std::ostream& operator<<(std::ostream& os, const cubeList& cl) {
    os << cl.numLiterals << std::endl;
    if (cl.empty()) {
        return os;
    }

    for (const auto& cube : cl.cl) {
        for (const auto& lit : cube) {
            switch (lit) {
                case cubeList::Dontcare:
                    os << '-';
                    break;
                case cubeList::Positive:
                    os << '1';
                    break;
                case cubeList::Negative:
                    os << '0';
                    break;
            }
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, cubeList& cl) {
    is >> cl.numLiterals;
    std::string line;
    // Ignore the newline character after the first integer
    std::getline(is, line);

    while (std::getline(is, line)) {
        cubeList::cube currentCube;
        for (char ch : line) {
            switch (ch) {
                case '-':
                    currentCube.push_back(cubeList::Dontcare);
                    break;
                case '1':
                    currentCube.push_back(cubeList::Positive);
                    break;
                case '0':
                    currentCube.push_back(cubeList::Negative);
                    break;
                default:
                    break;
            }
        }
        if (currentCube.size() == cl.numLiterals) {
            cl.cl.push_back(currentCube);
        }
    }
    return is;
}

cubeList operator>(cubeList::input in, const cubeList& cl) {
    cubeList::cubelist result;
    size_t index = in.first;
    cubeList::literal l = in.second;
    for (const auto& cube : cl.cl) {
        if (cube[index] == cubeList::Dontcare) {
            result.push_back(cube);
        } else if (cube[index] == l) {
            cubeList::cube new_cube(cube);
            new_cube[index] = cubeList::Dontcare;
            result.push_back(new_cube);
        } else continue;
    }
    return cubeList(cl.numLiterals, result);
}

cubeList operator*(cubeList::input in, const cubeList& cl) {
    cubeList::cubelist result;
    size_t index = in.first;
    cubeList::literal l = in.second;
    for (const auto& cube : cl.cl) {
        if (cube[index] == cubeList::Dontcare) {
            cubeList::cube new_cube(cube);
            new_cube[index] = l;
            result.push_back(new_cube);
        } else continue;
    }
    return cubeList(cl.numLiterals, result);
}

cubeList operator+(const cubeList& cl1, const cubeList& cl2) {
    cubeList::cubelist result;

    // Combine cubelists from cl1
    for (const auto& cube : cl1.cl) {
        result.push_back(cube);
    }

    // Combine cubelists from cl2
    for (const auto& cube : cl2.cl) {
        result.push_back(cube);
    }
    return cubeList(cl1.numLiterals, result);
}
