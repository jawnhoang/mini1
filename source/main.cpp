#include "../include/Facade.hpp"
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

using namespace std;

int main(int argc, char** argv) {
    // allow passing a single CSV file or a directory; default to fires folder
    std::string inputPath = (argc >= 2) ? argv[1] : std::string("../datasets/fires/");
    Facade facade;
    // parse datasets/fires directory and store all csv file data in a vector with thread count of choice
    vector<vector<string>> csvFilesData = facade.readCsv(inputPath, 1);

    // parse datasets/world_bank directory and store all csv file data in a vector  with thread count of choice
    // TODO



    //verify data is stored in csvFilesData 
    // int count = 0;
    // for (const auto& data : csvFilesData) {
    //     for(size_t i = 0; i < data.size(); ++i){
    //         cout << data[i] << ',' ;
    //         if (i != data.size() - 1) cout << "";
    //     }
    //     cout << endl;
    //     if(++count >= 10) break; // print only first 5 files' data
    // }
    

    //---------- Query BEGIN ------------
    //Fire Index:Column Desc 
    // 0:latitude, 1:longitude, 2:UTC, 3:Parameter, 4:Concentration, 5:Unit, 6:Raw Concentration, 7:Aqi, 8:Category, 9:Site Name, 10:Site Agency, 11: AQS ID, 12:Full AQS ID 


    // -- SINGLE THREAD QUERIES BEGIN---------

    //Q1: How many cases of ## reporting have missing Raw Concentration or AQI	@Raeeka
        // PM10
        // PM2.5
        // OZONE
        // CO
        // NO2
    // load ONLY the path the user passed (file or dir)
    auto q1Data = csvFilesData; // reuse the already-loaded input (file or dir)

    // now run Q1 on that
    auto stats = facade.getMissingRawAqiByParameter(q1Data, 3, 6, 7, 1);


    //std::cout << "parameter,total,missing_raw,missing_aqi\n";
    // Format results
    vector<pair<string,string>> q1Results;
    for (const auto& t : stats) {
        string key = std::get<0>(t);
        string val = "total=" + to_string(std::get<1>(t)) +
                    ", missing_raw=" + to_string(std::get<2>(t)) +
                    ", missing_aqi=" + to_string(std::get<3>(t));
        q1Results.emplace_back(key, val);
    }

    facade.printResults(q1Results);



    // Q2: In the Month of #, what site yielded the BEST and WORST AQI? Note: Lower AQI = better  @John
        // PM10 in Aug
        vector<int> columnIndx = {2, 3, 7};
        vector<string> columnData = {"08", "PM10"};
        vector<pair<string,string>> result = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData, 1);
        facade.printResults(result, 10);
        // PM2.5 in Sep
        // OZONE in Aug
        // CO in Sep
        // NO2 in Aug


    // Q3: What country codes are in what income level? @Raeeka
        // Low
        // Med
        // Upper
        // lower
        // high



    // Q4: Top 10 countries that grew in population between 1960 - 2023 @John

    // -- SINGLE THREAD QUERIES END ---------


    // -- MULTI THREAD QUERIES BEGIN ---------


    // -- MULTI THREAD QUERIES END   ---------


    //---------- Query END ------------


    return 0;
}