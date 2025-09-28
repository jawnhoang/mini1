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
    cout<< msg<< ": " << executionDuration << "ms" << endl;
    cout<<"------------------" << endl;
}