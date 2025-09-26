
#pragma once

#include <string>
#include <vector>


using namespace std;

class Facade {
    public:
        vector<vector<string>> readCsv(const string& roodDir, int num_threads);
        vector<pair<string,string>> getMaxAqiBasedOnParticulantsAndMonth(vector<vector<string>>& dataset, vector<int>& columnIndex, vector<string>& columnData, int num_threads);
        // vector<string> getMinAqiBasedOnParticulantsAndMonth(vector<int>& columnIndex, vector<string>& columnData, int num_threads);
        void printResults(vector<pair<string,string>>& results);
};