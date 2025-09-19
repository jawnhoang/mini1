#pragma once
#include "DataParser.hpp"

using namespace std;


class CsvParser : public DataParser{ //concret class inherits abstract class
    public:
        vector<vector<string>> read(const string& filePath) override;
};
