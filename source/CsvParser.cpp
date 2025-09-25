
#include "CsvParser.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <omp.h>
#include <utility>


using namespace std;

/**
 * Reads csv line by line into memory
 * Invokes num_threads threads to store data
 * 
 * Args: filePath, num_threads
 */
vector<vector<string>> CsvParser::read(const string& filePath, int num_threads) {
    ifstream file(filePath);
    string line;
    vector<string> vectorOfLines;

    while (getline(file, line)) {
        vectorOfLines.push_back(line);
    }

    vector<vector<string>> data (vectorOfLines.size());


    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < vectorOfLines.size(); i++){
        stringstream ss(vectorOfLines[i]);
        
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data[i] = std::move(row);

    }
    return data;



};

/**
 * Gathers all filepaths within a root directory
 * 
 * Args: rootDir
 */
vector<string> CsvParser::getFilePaths(const string& rootDir){
    vector<string> csvFilePaths;
    for (const auto& entry : filesystem::recursive_directory_iterator(rootDir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".csv") {
        csvFilePaths.push_back(entry.path().string());
    }
    }
    return csvFilePaths;
};
