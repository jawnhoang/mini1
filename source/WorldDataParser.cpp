#include "../include/WorldDataParser.hpp"
#include <iostream>
#include <string>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#endif

#include <algorithm>
#include <iomanip>
#include <cctype>

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

// Q3: group ISO3 country codes by income level (header-driven)
vector<pair<string,string>>
WorldDataParser::incomeGroupsByHeader(const vector<vector<string>>& wbData,
                                      const vector<string>& codeHeaderNames,
                                      const vector<string>& groupHeaderNames)
{
    if (wbData.empty()) return {};

    auto trimq = [](const string& s){
        size_t b=0,e=s.size();
        while (b<e && (s[b]==' '||s[b]=='\t')) ++b;
        while (e>b && (s[e-1]==' '||s[e-1]=='\t')) --e;
        if (e-b>=2 && s[b]=='"' && s[e-1]=='"'){ ++b; --e; }
        return string(s.begin()+b, s.begin()+e);
    };
    auto norm = [&](string s){
        s = trimq(s);
        // strip UTF-8 BOM if present
        if (s.size()>=3 && (unsigned char)s[0]==0xEF && (unsigned char)s[1]==0xBB && (unsigned char)s[2]==0xBF)
            s.erase(0,3);
        for (auto& ch : s) ch = (char)tolower((unsigned char)ch);
        return s;
    };
    auto strip = [](const string& x){
        string y; y.reserve(x.size());
        for (char ch: x) if (ch!=' ' && ch!='\t' && ch!='_' && ch!='/') y.push_back(ch);
        return y;
    };
    auto matches_any = [&](const string& cell, const vector<string>& targets){
        auto c = strip(norm(cell));
        for (auto& t : targets) if (c == strip(norm(t))) return true;
        return false;
    };

    // 1) resolve header row + columns from names provided by main
    int header_row = -1, codeCol=-1, groupCol=-1;
    int scan = min<int>(100, (int)wbData.size());
    for (int r=0; r<scan; ++r){
        const auto& row = wbData[r];
        for (int c=0; c<(int)row.size(); ++c){
            if (codeCol==-1  && matches_any(row[c], codeHeaderNames))  codeCol  = c;
            if (groupCol==-1 && matches_any(row[c], groupHeaderNames)) groupCol = c;
        }
        if (codeCol!=-1 && groupCol!=-1){ header_row = r; break; }
        codeCol = groupCol = -1;
    }
    if (header_row == -1) return {};

    // 2) bucket ISO3 country codes by income group label
    vector<string> low, lower_mid, upper_mid, high;

    for (int r=header_row+1; r<(int)wbData.size(); ++r){
        const auto& row = wbData[r];
        if (codeCol >= (int)row.size() || groupCol >= (int)row.size()) continue;
        string code = trimq(row[codeCol]);
        string grp  = trimq(row[groupCol]);
        if (code.empty() || grp.empty()) continue;

        if      (grp == "Low income")            low.push_back(code);
        else if (grp == "Lower middle income")   lower_mid.push_back(code);
        else if (grp == "Upper middle income")   upper_mid.push_back(code);
        else if (grp == "High income")           high.push_back(code);
    }

    auto join = [](const vector<string>& v){
        string out; out.reserve(v.size()*4);
        for (size_t i=0;i<v.size();++i){ if(i) out.push_back(','); out += v[i]; }
        return out;
    };

    vector<pair<string,string>> res;
    res.emplace_back("Low",          join(low));
    res.emplace_back("Lower middle", join(lower_mid));
    res.emplace_back("Upper middle", join(upper_mid));
    res.emplace_back("High",         join(high));
    return res;
}