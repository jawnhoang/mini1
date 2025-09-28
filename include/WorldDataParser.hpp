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
        // Q3: group ISO3 country codes by income level (header-driven)
        vector<pair<string,string>>
        incomeGroupsByHeader(const vector<vector<string>>& wbData,
                             const vector<string>& codeHeaderNames,
                             const vector<string>& groupHeaderNames);
    
};
