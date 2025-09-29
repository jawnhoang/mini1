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
    //Population dataset
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
    cout << "Single Thread Query1: How many cases of Particulants reporting have missing Raw Concentration and AQI?" << endl;
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



    // Q2: In the Month of #, what site yielded the BEST AQI? Note: Lower AQI = better  @John
    cout<< "Single Thread Query 2: In the Month of Aug or Sep, what sites yielded the BEST AQI?" <<endl;
        Facade querySpecific;
        querySpecific.startTimer();
        // // PM10 in Aug
        facade.startTimer();
        vector<int> columnIndx = {2, 3, 7}; // utc, param, aqi
        vector<string> columnData = {"08", "PM10"};
        cout<< "\nTop 10 Sites with the BEST PM10 AQI readings in August: "<< endl;
        vector<pair<string,string>> PM10 = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData, 1);
        facade.printResults(PM10, 10);
        facade.stopTimer();
        string msg1 = "Single Thread execution duration";
        facade.printTimeSummary(msg1);
        // PM2.5 in Sep
        facade.startTimer();
        vector<string> columnData2 = {"09", "PM2.5"};
        cout<< "\nTop 10 Sites with the BEST PM2.5 AQI readings in September: "<< endl;
        vector<pair<string,string>> PM25 = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData2, 1);
        facade.printResults(PM25, 10);
        facade.stopTimer();
        string msg2 = "Single Thread execution duration";
        facade.printTimeSummary(msg2);

        // OZONE in Aug
        facade.startTimer();
        vector<string> columnData3 = {"08", "OZONE"};
        cout<< "\nTop 10 Sites with the BEST OZONE AQI readings in August: "<< endl;
        vector<pair<string,string>> OZONE = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData3, 1);
        facade.printResults(OZONE, 10);
        facade.stopTimer();
        string msg3 = "Single Thread execution duration";
        facade.printTimeSummary(msg3);
        // CO in Sep
        facade.startTimer();
        vector<string> columnData4 = {"09", "CO"};
        cout<< "\nTop 10 Sites with the BEST CO AQI readings in September: "<< endl;
        vector<pair<string,string>> CO = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData4, 1);
        facade.printResults(CO, 10);
        facade.stopTimer();
        string msg4 = "Single Thread execution duration";
        facade.printTimeSummary(msg4);
        // NO2 in Aug
        facade.startTimer();
        vector<string> columnData5 = {"08", "NO2"};
        cout<< "\nTop 10 Sites with the BEST NO2 AQI readings in August: "<< endl;
        vector<pair<string,string>> NO2 = facade.getMaxAqiBasedOnParticulantsAndMonth(csvFilesData, columnIndx, columnData5, 1);
        facade.printResults(NO2, 10);
        facade.stopTimer();
        string msg5 = "Single Thread execution duration";
        facade.printTimeSummary(msg5);

        querySpecific.stopTimer();
        string msg6 = "Overall execution for Query 2:";
        querySpecific.printTimeSummary(msg6);

    // Q3: What country codes are in what income level? @Raeeka
        // Low
        // Med
        // Upper
        // lower
        // high
    // --- Q3 BEGIN (WB metadata-driven; minimal main, logic in Facade) ---
    // Use a separate metadata COUNTRY file for income groups (do NOT reuse the population CSV used by Q4)
    // Allow passing it as argv[2]; otherwise default to the expected metadata filename.
    std::string wbMetaPath = (argc >= 3)
        ? std::string(argv[2])
        : std::string("../datasets/world_bank/Metadata_Country_API_SP.POP.TOTL_DS2_en_csv_v2_3401680.csv");

    // Reuse the same facade instance for WB work
    auto wbMetaData = facade2.readCsv(wbMetaPath, 1);

    // We will implement this Facade method next; keeping main tiny and pattern-consistent.
    // Header names are provided here so the code is generic for future datasets.
    vector<string> codeHeaders  = {"Country Code","CountryCode","ISO3 Code","Country ISO3 Code","Code"};
    vector<string> groupHeaders = {"IncomeGroup","Income Group","Income Level","IncomeLevel"};

    auto wbGroups = facade2.getWorldBankIncomeGroupsByHeader(wbMetaData, codeHeaders, groupHeaders, 1);
   
   
    //facade2.printResults(wbGroups);
    // Pretty print with group counts and wrapped lines
    std::cout << "\n[WB-Q3] ISO3 codes by income level\n---------------------------------\n";
    for (const auto& kv : wbGroups) {
        const std::string& label = kv.first;
        const std::string& codes = kv.second; // already wrapped above
        // count items = commas + 1 (if non-empty)
        std::size_t count = 0; for (char ch : codes) if (ch == ',') ++count; if (!codes.empty()) ++count;
        std::cout << label << " (" << count << "):\n" << codes << "\n\n";
    }
    // --- Q3 END ---

/*

    // Q4: Average population between 1960 - 2023 for all countries @John
    //start from row 5 for start of data
    // 1960 - 2023: col 5 - 68
    cout<<"Single Thread Query 4: What is the average population size of each country between 1960-2023?" <<endl;
    vector<int> wbColIdx = {0, 5, 68 }; // cols: country name, 1960, 1968
    int dataRowStart = 5;
    facade2.startTimer();
    facade2.getAvgPop(wbData, wbColIdx, dataRowStart, 1);
    facade2.stopTimer();
    string wbMsg = "\nSingle Thread execution for Average Population 1960-2023: ";
    facade2.printTimeSummary(wbMsg);


*/
    // -- SINGLE THREAD QUERIES END ---------


    // -- MULTI THREAD QUERIES BEGIN ---------

    // Q2: In the Month of #, what site yielded the BEST and WORST AQI? Note: Lower AQI = better  @John
    cout<<"Multi Thread Query 2: In the Month of Aug or Sep, what sites yielded the BEST AQI?" <<endl;
    Facade multiFacade;
    vector<vector<string>> csvDataMulti = multiFacade.readCsv(inputPath, 1);
    Facade querySpecificMulti;
    querySpecificMulti.startTimer();
    // // PM10 in Aug
    multiFacade.startTimer();
    vector<int> columnIndxMulti = {2, 3, 7}; // utc, param, aqi
    vector<string> columnDataMulti = {"08", "PM10"};
    cout<< "\nTop 10 Sites with the BEST PM10 AQI readings in August: "<< endl;
    vector<pair<string,string>> PM10Multi = multiFacade.getMaxAqiBasedOnParticulantsAndMonth(csvDataMulti, columnIndxMulti, columnDataMulti, 2);
    multiFacade.printResults(PM10Multi, 10);
    multiFacade.stopTimer();
    string msg1Multi = "Multi Thread execution duration";
    multiFacade.printTimeSummary(msg1Multi);
    // PM2.5 in Sep
    multiFacade.startTimer();
    vector<string> columnData2Multi = {"09", "PM2.5"};
    cout<< "\nTop 10 Sites with the BEST PM2.5 AQI readings in September: "<< endl;
    vector<pair<string,string>> PM25Multi = multiFacade.getMaxAqiBasedOnParticulantsAndMonth(csvDataMulti, columnIndxMulti, columnData2Multi, 2);
    multiFacade.printResults(PM25Multi, 10);
    multiFacade.stopTimer();
    string msg2Multi = "Multi Thread execution duration";
    multiFacade.printTimeSummary(msg2Multi);

    // OZONE in Aug
    multiFacade.startTimer();
    vector<string> columnData3Multi = {"08", "OZONE"};
    cout<< "\nTop 10 Sites with the BEST OZONE AQI readings in August: "<< endl;
    vector<pair<string,string>> OZONEMulti = multiFacade.getMaxAqiBasedOnParticulantsAndMonth(csvDataMulti, columnIndxMulti, columnData3Multi, 2);
    multiFacade.printResults(OZONEMulti, 10);
    multiFacade.stopTimer();
    string msg3Multi = "Multi Thread execution duration";
    multiFacade.printTimeSummary(msg3Multi);
    // CO in Sep
    multiFacade.startTimer();
    vector<string> columnData4Multi = {"09", "CO"};
    cout<< "\nTop 10 Sites with the BEST CO AQI readings in September: "<< endl;
    vector<pair<string,string>> COMulti = multiFacade.getMaxAqiBasedOnParticulantsAndMonth(csvDataMulti, columnIndxMulti, columnData4Multi, 2);
    multiFacade.printResults(COMulti, 10);
    multiFacade.stopTimer();
    string msg4Multi = "Multi Thread execution duration";
    multiFacade.printTimeSummary(msg4Multi);
    // NO2 in Aug
    multiFacade.startTimer();
    vector<string> columnData5Multi = {"08", "NO2"};
    cout<< "\nTop 10 Sites with the BEST NO2 AQI readings in August: "<< endl;
    vector<pair<string,string>> NO2Multi = multiFacade.getMaxAqiBasedOnParticulantsAndMonth(csvDataMulti, columnIndxMulti, columnData5Multi, 2);
    multiFacade.printResults(NO2Multi, 10);
    multiFacade.stopTimer();
    string msg5Multi = "Multi Thread execution duration";
    multiFacade.printTimeSummary(msg5Multi);

    querySpecificMulti.stopTimer();
    string msg6Multi = "Overall execution for Query 2 with Multi Threading:";
    querySpecificMulti.printTimeSummary(msg6Multi);

    // Q4: Average population between 1960 - 2023 for all countries @John
    //start from row 5 for start of data
    // 1960 - 2023: col 5 - 68
    cout<<"Multi Thread Query 4: What is the average population size of each country between 1960-2023?" <<endl;
    facade2.startTimer();
    facade2.getAvgPop(wbData, wbColIdx, dataRowStart, 2);
    facade2.stopTimer();
    string wbMsgMulti = "Multi Thread execution for Average Population 1960-2023: ";
    facade2.printTimeSummary(wbMsgMulti);

    // -- MULTI THREAD QUERIES END   ---------


    //---------- Query END ------------


    return 0;
}