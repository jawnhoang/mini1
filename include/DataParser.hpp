
#pragma once // header included once during compilation to avoid duplicate class definitions
#include <iostream>
#include <vector> // similar to arrayLists
//header file to provide data across all code
//abstract class / interface (in java terms)

//shorthand commonly used namespaces so no need to keep declaring 'std::'
using namespace std;

class DataParser{
    public:
        virtual ~DataParser() = default;
        virtual vector<vector<string>> read(const string& filepath) = 0;
};

