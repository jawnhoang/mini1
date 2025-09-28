#pragma once

#include <iostream>
#include <string>
#include <vector>


using namespace std;

class WorldDataParser{
    private:
        vector<pair<string, float>> CountryToAvgPop;
    
    public:
        void calculateAvgPop1930_1968(vector<vector<string>>& csvData, vector<int>& columnIdx, int& rowStart,int num_threads);
        void printAvgPopSummary();

    
};
