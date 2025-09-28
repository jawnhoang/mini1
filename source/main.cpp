#include "../include/Facade.hpp"
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <chrono>

using namespace std;

int main(int argc, char** argv) {
    // allow passing a single CSV file or a directory; default to fires folder
    std::string inputPath = (argc >= 2) ? argv[1] : std::string("../datasets/fires/");
    Facade facade;
    // parse datasets/fires directory and store all csv file data in a vector with thread count of choice
    vector<vector<string>> csvFilesData = facade.readCsv(inputPath, 1);

    // parse datasets/world_bank directory and store all csv file data in a vector  with thread count of choice
    // TODO
    Facade facade2;
    string wbPath = "../datasets/world_bank/API_SP.POP.TOTL_DS2_en_csv_v2_3401680.csv";
    vector<vector<string>> wbData = facade2.readCsv(wbPath, 1);



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
        // Facade querySpecific;
        // querySpecific.startTimer();
        // // PM10 in Aug
        // facade.startTimer();
        // vector<int> columnIndx = {2, 3, 7}; // utc, param, aqi
        // vector<string> columnData = {"08", "PM10"};
        // cout<< "\nTop 10 Sites with the BEST PM10 AQI readings in August: "<< endl;
        // vector<pair<string,string>> PM10 = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData, 1);
        // facade.printResults(PM10, 10);
        // facade.stopTimer();
        // string msg1 = "Single Thread execution duration";
        // facade.printTimeSummary(msg1);
        // // PM2.5 in Sep
        // facade.startTimer();
        // vector<string> columnData2 = {"08", "PM2.5"};
        // cout<< "\nTop 10 Sites with the BEST PM2.5 AQI readings in August: "<< endl;
        // vector<pair<string,string>> PM25 = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData2, 1);
        // facade.printResults(PM25, 10);
        // facade.stopTimer();
        // string msg2 = "Single Thread execution duration";
        // facade.printTimeSummary(msg2);

        // // OZONE in Aug
        // facade.startTimer();
        // vector<string> columnData3 = {"08", "OZONE"};
        // cout<< "\nTop 10 Sites with the BEST OZONE AQI readings in August: "<< endl;
        // vector<pair<string,string>> OZONE = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData3, 1);
        // facade.printResults(OZONE, 10);
        // facade.stopTimer();
        // string msg3 = "Single Thread execution duration";
        // facade.printTimeSummary(msg3);
        // // CO in Sep
        // facade.startTimer();
        // vector<string> columnData4 = {"08", "CO"};
        // cout<< "\nTop 10 Sites with the BEST CO AQI readings in August: "<< endl;
        // vector<pair<string,string>> CO = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData4, 1);
        // facade.printResults(CO, 10);
        // facade.stopTimer();
        // string msg4 = "Single Thread execution duration";
        // facade.printTimeSummary(msg4);
        // // NO2 in Aug
        // facade.startTimer();
        // vector<string> columnData5 = {"08", "NO2"};
        // cout<< "\nTop 10 Sites with the BEST NO2 AQI readings in August: "<< endl;
        // vector<pair<string,string>> NO2 = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData5, 1);
        // facade.printResults(NO2, 10);
        // facade.stopTimer();
        // string msg5 = "Single Thread execution duration";
        // facade.printTimeSummary(msg5);

        // querySpecific.stopTimer();
        // string msg6 = "Overall Query execution:";
        // querySpecific.printTimeSummary(msg6);

    // Q3: What country codes are in what income level? @Raeeka
        // Low
        // Med
        // Upper
        // lower
        // high



    // Q4: Top 10 countries that grew in population between 1960 - 2023 @John
    //start from row 5 for start of data
    // 1960 - 2023: col 5 - 68
    vector<int> wbColIdx = {0, 5, 68 }; // cols: country name, 1960, 1968
    int dataRowStart = 5;
    facade2.startTimer();
    facade2.getAvgPop(wbData, wbColIdx, dataRowStart, 1);
    facade2.stopTimer();
    string wbMsg = "\nSingle Thread execution: ";
    facade2.printTimeSummary(wbMsg);

    // -- SINGLE THREAD QUERIES END ---------


    // -- MULTI THREAD QUERIES BEGIN ---------


    // -- MULTI THREAD QUERIES END   ---------


    //---------- Query END ------------


    return 0;
}