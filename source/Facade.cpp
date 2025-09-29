#include "Facade.hpp"
#include "CsvParser.hpp"
#include "Stopwatch.hpp"
#include "WorldDataParser.hpp"
#include <iomanip>
#include <algorithm>
#include <set>

using namespace std;

/**
 * Encapsulates calls to CsvReader to read and store CSV
 * 
 * Args: rootDir, num_threads
 * 
 */
vector<vector<string>> Facade::readCsv(const string& roodDir, int num_threads) {
    // Heuristic: if the path ends with .csv (case-insensitive), treat as single file
    if (roodDir.size() >= 4) {
        auto ends_with_icase = [](const std::string& s, const char* suf){
            size_t n = std::strlen(suf);
            if (s.size() < n) return false;
            for (size_t i = 0; i < n; ++i) {
                char a = std::tolower(static_cast<unsigned char>(s[s.size()-n+i]));
                char b = std::tolower(static_cast<unsigned char>(suf[i]));
                if (a != b) return false;
            }
            return true;
        };
        if (ends_with_icase(roodDir, ".csv")) {
            CsvParser csvParserSingleBySuffix;
            auto rows = csvParserSingleBySuffix.read(roodDir, num_threads);
            // Optional: small debug line; keep for now, easy to remove later
            std::cerr << "[Facade] Single-file mode by suffix: " << roodDir << " (rows=" << rows.size() << ")\n";
            return rows;
        }
    }
    //get filepaths of ALL csvs in specified directory
    CsvParser csvParser;
    vector<string> filePaths = csvParser.getFilePaths(roodDir);
    std::cerr << "[Facade] Directory mode: found " << filePaths.size() << " CSV files under " << roodDir << "\n";
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

    //iterate through rows of data based on column index and take row that matches columnData conditions into consideration to crunch data
    #pragma omp parallel num_threads(num_threads) // establishes how many threads will run this section
    {
        vector<vector<string>> individualThreadStorage; //give a thread it's own storage to avoid race conditions
        #pragma omp for // divide work among num_threads threads
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
        #pragma omp barrier // lets threads finish the first half first. Fully write to aqiData before moving onto the next part to prevent race conditions

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
    //once threads have saved to sites, sort site to remove any duplicate rows.
    set<pair<string,string>> uniqueSites(sites.begin(), sites.end());
    sites.assign(uniqueSites.begin(), uniqueSites.end());

    return sites;
};


// vector<string> Facade::getMinAqiBasedOnParticulantsAndMonth(vector<int>& columnIndex, vector<string>& columnData, int num_threads){
//     throw("empty");
// };


void Facade::printResults(vector<pair<string,string>>& results, int sortRange){
    // cout<< results.size();
    if(sortRange > 0){
        sort(results.begin(), results.end(),
            [](const auto& first, const auto& last){
                return first.first < last.first;
            });
            int range = min(sortRange, (int)results.size());
                for(int i = 0; i < range; ++i){   
            cout << left  << setw(20) << ("Site: " + results[i].second) << right << setw(30) << "AQI: " << results[i].first << endl;
        }
    }else{
        for(const auto& e : results){
             cout << left  << setw(20) << ("Site: " + e.second) << right << setw(30) << "AQI: " << e.first << endl;
        }
    }
};

std::vector<std::tuple<std::string, size_t, size_t, size_t>>
Facade::getMissingRawAqiByParameter(std::vector<std::vector<std::string>>& dataset,
                                     int paramCol, int rawCol, int aqiCol, int num_threads) {
    // fixed parameter order for output
    const std::string params[5] = {"PM10","PM2.5","OZONE","CO","NO2"};
    auto idx_of = [&](const std::string& p)->int{
        for (int i=0;i<5;++i) if (dataset.size()>0 && p == params[i]) return i;
        for (int i=0;i<5;++i) if (p == params[i]) return i; // fallback
        return -1;
    };

    // counts[5] = {total, missing_raw, missing_aqi}
    std::vector<std::array<size_t,3>> counts(5, {0,0,0});

    // helper: strip surrounding quotes ("...") if present
    auto dequote = [](const std::string& s) -> std::string {
        if (s.size() >= 2 && s.front()=='"' && s.back()=='"')
            return s.substr(1, s.size()-2);
        return s;
    };

    auto trim_copy = [](std::string s){
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
        return s;
    };
    auto is_missing = [&](const std::string& s0){
        std::string s = trim_copy(s0);
        if (s.empty()) return true;
        std::string u; u.reserve(s.size());
        for (unsigned char c: s) u.push_back(std::toupper(c));
        if (u=="NA"||u=="N/A"||u=="NULL"||u=="NAN"||u==".") return true;
        if (s.rfind("-999",0)==0 || s.rfind("-9999",0)==0) return true;
        return false;
    };

    #pragma omp parallel num_threads(num_threads)
    {
        std::array<size_t,3> local[5];
        for (int i=0;i<5;++i) local[i] = {0,0,0};

        #pragma omp for
        for (int r = 0; r < (int)dataset.size(); ++r) {
            const auto& row = dataset[r];
            if ((int)row.size() <= aqiCol) continue;
            const std::string pnorm = trim_copy(dequote(row[paramCol]));
            int k = idx_of(pnorm);
            if (k < 0) continue; // ignore other parameters
            local[k][0]++;
            if (is_missing(dequote(row[rawCol]))) local[k][1]++;
            if (is_missing(dequote(row[aqiCol]))) local[k][2]++;
        }

        #pragma omp critical
        {
            for (int i=0;i<5;++i) {
                counts[i][0] += local[i][0];
                counts[i][1] += local[i][1];
                counts[i][2] += local[i][2];
            }
        }
    }

    std::vector<std::tuple<std::string,size_t,size_t,size_t>> out; out.reserve(5);
    for (int i=0;i<5;++i) {
        out.emplace_back(params[i], counts[i][0], counts[i][1], counts[i][2]);
    }
    return out;
}


void Facade::startTimer(){
    timer.startTimer();
}

void Facade::stopTimer(){
    timer.stopTimer();

}

void Facade::printTimeSummary(string& msg){
    timer.printTimeSummary(msg);
}

void Facade::getAvgPop(vector<vector<string>>& csvData, vector<int>& columnIdx, int& rowStart,int num_threads){
    wdParser.calculateAvgPop1930_1968(csvData, columnIdx, rowStart, num_threads);
    wdParser.printAvgPopSummary();
}

// Q3 wrapper: forward to WorldDataParser (header-driven)
std::vector<std::pair<std::string,std::string>>
Facade::getWorldBankIncomeGroupsByHeader(std::vector<std::vector<std::string>>& wbData,
                                         const std::vector<std::string>& codeHeaderNames,
                                         const std::vector<std::string>& groupHeaderNames,
                                         int /*num_threads*/) {
    WorldDataParser wdp;
    return wdp.incomeGroupsByHeader(wbData, codeHeaderNames, groupHeaderNames);
}