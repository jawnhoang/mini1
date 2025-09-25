#include "Facade.hpp"
#include <iostream>
#include <filesystem>

using namespace std;

int main() {
    Facade facade;
    // parse datasets/fires directory and store all csv file data in a vector with thread count of choice
    vector<vector<string>> csvFilesData = facade.readCsv("../datasets/fires/", 1);

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
    //Q1: How many cases of ## reporting have missing Raw Concentration or AQI	@Raeeka
        // PM10
        // PM2.5
        // OZONE
        // CO
        // NO2




    // Q2: In the Month of #, what site yielded the BEST and WORST AQI? Note: Lower AQI = better  @John
        // PM10
        // PM2.5
        // OZONE
        // CO
        // NO2


    // Q3: What country codes are in what income level? @Raeeka
        // Low
        // Med
        // Upper
        // lower
        // high



    // Q4: Top 10 countries that grew in population between 1960 - 2023 @John


    //---------- Query END ------------


    return 0;
}