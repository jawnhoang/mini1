#include "Facade.hpp"
#include <iostream>
#include <filesystem>

using namespace std;

int main() {
    Facade facade;

    // parse datasets/fires directory and store all csv file paths in a vector
    vector<vector<string>> csvFilesData = facade.readCsv("../datasets/fires/", 1);

    // csvFiles now contains all CSV file paths under datasets/fires
    //verify data is stored in csvFilesData 
    // int count = 0;
    // for (const auto& data : csvFilesData) {
    //     for(size_t i = 0; i < data.size(); ++i){
    //         cout << data[i] << ',' ;
    //         if (i != data.size() - 1) cout << "";
    //     }
    //     cout << endl;
    //     if(++count >= 100) break; // print only first 5 files' data
    // }

    //parallel process line by line into a vector<vector<string>>
}