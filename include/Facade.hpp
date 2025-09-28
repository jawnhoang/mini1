
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <Stopwatch.hpp>
#include <WorldDataParser.hpp>

using namespace std;

class Facade {
    struct MissingCounts {
        size_t total = 0;
        size_t missing_raw = 0;
        size_t missing_aqi = 0;
    };
    private:
        Stopwatch timer;
        WorldDataParser wdParser;

    public:
        vector<vector<string>> readCsv(const string& roodDir, int num_threads);
        vector<pair<string,string>> getMaxAqiBasedOnParticulantsAndMonth(vector<vector<string>>& dataset, vector<int>& columnIndex, vector<string>& columnData, int num_threads);
        // vector<string> getMinAqiBasedOnParticulantsAndMonth(vector<int>& columnIndex, vector<string>& columnData, int num_threads);
        std::vector<std::tuple<std::string, size_t, size_t, size_t>>
        getMissingRawAqiByParameter(std::vector<std::vector<std::string>>& dataset,
                                    int paramCol, int rawCol, int aqiCol, int num_threads);

        void printResults(vector<pair<string,string>>& results, int sortRange = -1);
        void startTimer();
        void stopTimer();
        void printTimeSummary(string& msg);

        void getAvgPop(vector<vector<string>>& csvData, vector<int>& columnIdx, int& rowStart,int num_threads);
    
};