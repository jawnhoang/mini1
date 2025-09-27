
#include "Facade.hpp"
#include "CsvParser.hpp"
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * Encapsulates calls to CsvReader to read and store CSV
 * 
 * Args: rootDir, num_threads
 * 
 */
vector<vector<string>> Facade::readCsv(const string& roodDir, int num_threads) {
    //get filepaths of ALL csvs in specified directory
    CsvParser csvParser;
    vector<string> filePaths = csvParser.getFilePaths(roodDir);
    // cout << filePaths.size() << endl;

    //read
    vector<vector<string>> csvData;
    for (const string& files : filePaths) {
        vector<vector<string>> dataFromFile = csvParser.read(files, num_threads);
        csvData.insert(csvData.end(), dataFromFile.begin(), dataFromFile.end());
    }
    // vector<vector<string>> csvData;
    // cout<<filePaths[1]<<endl;
    // csvData = csvParser.read(filePaths[1], 1);
    // cout<<csvData.size()<<endl;
    return csvData;
}



vector<pair<string,string>> Facade::getMaxAqiBasedOnParticulantsAndMonth(vector<vector<string>>& dataset, vector<int>& columnIndex, vector<string>& columnData, int num_threads){
    vector<vector<string>> aqiData;
    vector<pair<string,string>> sites;

    //iterate through data based on column index and take row that matches columnData conditions into consideration to crunch data
    #pragma omp parallel num_threads(num_threads)
    {
        vector<vector<string>> individualThreadStorage; //give a thread it's own storage to avoid race conditions
        #pragma omp for
        for(int i = 0; i < dataset.size(); ++i){
            const auto& row = dataset[i];
            // cout << row[columnIndex[0]].substr(6,2)<< "==" << columnData[0] << endl;
            if(row[columnIndex[0]].substr(6,2) == columnData[0]){   //if row[UTC] contains desired month
                // cout<<row[columnIndex[1]].substr(1,columnData[1].size()) << " : " << columnData[1] << endl;
                if(row[columnIndex[1]].find(columnData[1]) != std::string::npos){           // if row[Parameter] contains desired particulant
                    // cout<<row[columnIndex[1]].substr(1,columnData[1].size()) << " : " << columnData[1] << endl;
                    individualThreadStorage.push_back(row);         //Store row
                }
                
            }

        }
        //join threads
        #pragma omp critical
        // cout << "storing" << endl;
        aqiData.insert(aqiData.end(), individualThreadStorage.begin(), individualThreadStorage.end());
        // cout<< aqiData.size() <<endl;
        individualThreadStorage.clear();

        vector<pair<string,string>> individualThreadStorage2; //redeclare local storage but only as a 1D array
        
        #pragma omp for
        for(int i = 0; i < aqiData.size(); ++i){
            const auto& cell = aqiData[i];
            //domain knowledge : 0-50 AQI == GOOD/BEST
            double aqiReading;
            // cout<< cell[columnIndex[3]] << endl;
            try{
                aqiReading = stod(cell[columnIndex[3]].substr(1,5));
                // cout<<aqiReading <<endl;
            }catch(...){
                continue;
            }
            if(aqiReading < 50.00){
                individualThreadStorage2.emplace_back(cell[columnIndex[3]], cell[9]); // store AQI value and Site name

            }
        }
        #pragma omp critical
        sites.insert(sites.end(), individualThreadStorage2.begin(), individualThreadStorage2.end());

    }

    return sites;
};


// vector<string> Facade::getMinAqiBasedOnParticulantsAndMonth(vector<int>& columnIndex, vector<string>& columnData, int num_threads){
//     throw("empty");
// };


void Facade::printResults(vector<pair<string,string>>& results, int sortRange = -1){
    cout<< results.size();
    if(sortRange > 0){
        sort(results.begin(), results.end(),
            [](const auto& first, const auto& last){
                return first.first < last.first;
            });
    }

    for(const auto& e : results){   
        cout << left  << setw(20) << ("Site: " + e.second) << right << setw(30) << "AQI: " << e.first << endl;
    }
};
