
#include "CsvParser.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <omp.h>

using namespace std;

vector<vector<string>> CsvParser::read(const string& filePath){
    ifstream file(filePath);
    string line;
    vector<string> vectorOfLines;

    while (getline(file, line)) {
        vectorOfLines.push_back(line);
    }

    vector<vector<string>> data (line.size());


    #pragma omp parallel for
    for (int i = 0; i < line.size(); i++){
        stringstream ss(line[i]);
        
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data[i] = move(row);

    }

    return data;



};

