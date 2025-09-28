#pragma once

#include <iostream>
#include <chrono>

using namespace std;

class Stopwatch {
    private:
        chrono::high_resolution_clock::time_point start;
        chrono::high_resolution_clock::time_point stop;


    public:
        void startTimer();
        void stopTimer();
        void printTimeSummary(string& msg);
};