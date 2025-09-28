#include "../include/WorldDataParser.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "omp.h"
#include <algorithm>
#include <iomanip>

using namespace std;

void WorldDataParser::calculateAvgPop1930_1968(vector<vector<string>>& csvData, vector<int>& columnIdx, int& rowStart, int num_threads){
    //get Subset Data
    int totYears = columnIdx[2] - columnIdx[1] + 1;
    #pragma omp parallel num_threads(num_threads)
    {
        vector<pair<string,float>> individualThreadStorage;
        #pragma omp for
        for(int row = rowStart ; row < csvData.size(); ++row){
            // cout << csvData[rowStart][columnIdx[2]] << endl;
            long long result = 0;
            for(int cell = columnIdx[1]; cell <= columnIdx[2]; ++cell){
                string trim = csvData[row][cell];
                if(trim.back() == '\r'){
                    trim.pop_back();
                }
                trim.erase(remove(trim.begin(), trim.end(), '"'), trim.end());
                if(!trim.empty()){
                    // cout<<trim<<endl;
                    try{
                        result += stoll(trim);
                    }catch(...){
                        continue;
                    }
                }
            }
            float avg = result / totYears;
            individualThreadStorage.emplace_back(csvData[row][columnIdx[0]], avg);

        }
        #pragma omp critical
        CountryToAvgPop.insert(CountryToAvgPop.end(), individualThreadStorage.begin(), individualThreadStorage.end());
    }

}

void WorldDataParser::printAvgPopSummary(){
    //Order countries by alpha
    sort(CountryToAvgPop.begin(), CountryToAvgPop.end(), 
        [](const auto& a, const auto&b){
            return a.first < b.first;
        });
    cout<< "\nAverage Population of Every Country from 1960 to 2023: " <<endl;
    cout<<"---------------------"<<endl;
    for(const auto& e : CountryToAvgPop){
        cout<< fixed << setprecision(2) << e.first << right << setw(30) << (e.second) << endl;
    }
}