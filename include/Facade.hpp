
#pragma once

#include <string>
#include <vector>


using namespace std;

class Facade {
    public:
        vector<vector<string>> readCsv(const string& roodDir, int num_threads);
        int queryData(const string& condition);
};