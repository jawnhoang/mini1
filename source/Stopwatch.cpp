#include <Stopwatch.hpp>

#include <iostream>
#include <chrono>

using namespace std;

void Stopwatch::startTimer(){
    start = chrono::high_resolution_clock::now();
}

void Stopwatch::stopTimer(){
    stop = chrono::high_resolution_clock::now();
}

void Stopwatch::printTimeSummary(string& msg){
    auto executionDuration = chrono::duration_cast<chrono::milliseconds>(stop-start).count();
    if(executionDuration < 2){
        auto executionDurationNS = chrono::duration_cast<chrono::nanoseconds>(stop-start).count();
        cout<< msg<< ": " << executionDurationNS << "ns" << endl;
    }else{
        cout<< msg<< ": " << executionDuration << "ms" << endl;
    }
    cout<<"------------------" << endl;
}