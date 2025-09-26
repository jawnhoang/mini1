
#pragma once

#include <string>
#include <vector>


using namespace std;

class Facade {
    public:
        vector<vector<string>> readCsv(const string& roodDir, int num_threads);
        int getMaxAqiBasedOnParticulants(vector<string> columnData, int num_threads);
        int getMinAqiBasedOnParticulants(vector<string> columnData, int num_threads);
};