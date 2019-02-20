#include <iostream>
#include <set>
#include <random>
#include <cmath>
#include <vector>

#include "util.hpp"

using namespace std;

int main() {
    // This is an example of runtime benchmarking
    // TODO (final): Replace the following with a benchmarking comparison
    // of two data structures, as described in the project instructions

    int numTrials = 10;
    int dataSize = 500;
    int stepSize = 500;
    int currentStep;
    int numSteps = 20;
    
    vector<long long> times(numTrials, 0);

    Timer t;

    default_random_engine generator;
    uniform_real_distribution<double> distribution(-1.0, 1.0);

// Run benchmarks and time each trial
    for (int trialIndex = 0; trialIndex < numTrials; trialIndex++) {
        // Set a new random seed
        generator.seed(1 + trialIndex);


        double total = 0.0;
        for (int dataIndex = 0; dataIndex < numSteps; dataIndex++) {
            double currentRandom = distribution(generator);
            total += currentRandom;
            currentStep = dataSize + (stepSize * dataIndex);

            //BST<double> bst;
            set<double> set;
            for (int inserting = 0; inserting < currentStep; inserting++){
                set.insert(currentRandom);
            }
            t.begin_timer();
            /*
            BSTIterator<double> it = bst.begin();
            for (it; it != bst.end(); it++){
                bst.find(*it);
            }*/
            double element = currentRandom;
            auto search = set.find(element);
            times[dataIndex] = t.end_timer() + times[dataIndex];
        }
    }

    //Compute statistics from trial times
    double totalTime = 0.0;
    for (int i = 0; i < numSteps; i++) {
        times[i] = times[i] / (double)numTrials;
cout << times[i] << endl;
    }
    /*
    double meanTime = totalTime / ((double) numTrials);
    cout << "Elapsed time: " << totalTime << " nanoseconds ("
        << (totalTime / 1.0e9) << " seconds)" << endl;
    cout << "Mean: " << meanTime << " nanoseconds per trial" << endl;
    */
    return 0;
}
