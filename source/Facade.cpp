
#include "Facade.hpp"
#include "CsvParser.hpp"


using namespace std;

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