//
// Created by Mark Hsu on 2019-07-08.
//

#include "Statistic.h"
#include "GA.h"
#include <iostream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

int cntOverweight = 0;
int totalValue = 0;
int bestValue = 0;
int bestWeight = 0;
int bestFitness = 0;
int bestCase[10];
parent_t *best;

bool isOverweight(int w) {
    return w > KNAPSACK_SIZE;
}

void statistic() {
    best = getResult();

    if (isOverweight(best->weight)) {
        cntOverweight++;
        if (DEBUG_MODE)
            cout << "Overweight!!!" << endl;
    }

    totalValue += best->value;

    if (best->fitness > bestFitness) {
        bestWeight = best->weight;
        bestValue = best->value;
        bestFitness = best->fitness;

        int *ptrBestCase = getResultCnt();

        for (int k = 0; k < 10; k++) {
            bestCase[k] = *(ptrBestCase + k);
        }
    }
}

void finalResult() {
    cout << "==================== STATISTIC ====================\n";
    cout << "Knapsack size: " << KNAPSACK_SIZE << endl;
    cout << "Round: " << ROUND << endl;
    cout << "Generation: " << GENERATION << endl;
    cout << "Population: " << POPULATION_SIZE << endl;
    cout << "Mutation rate: " << MUTATION_RATE << "%" << endl;
    cout << "Punishment coefficient: " << ALPHA << endl << endl;


    cout << setw(17) << "Overweight count" << " |"
         << setw(16) << "Overweight rate" << " |"
         << setw(17) << "Best case weight" << " |"
         << setw(11) << "Avg. value" << " |"
         << setw(11) << "Best value" << " |"
         << setw(13) << "Best fitness" << endl;

    cout << setw(17) << cntOverweight << " |"
         << setw(14) << fixed << setprecision(2) << cntOverweight / (float) ROUND * 100
         << setw(2) << " %" << " |"
         << setw(17) << bestWeight << " |"
         << setw(11) << setprecision(2) << totalValue / (float) ROUND << " |"
         << setw(11) << bestValue << " |"
         << setw(13) << bestFitness << endl;

    cout << "Best case: ";
    for (int k = 0; k < 10; k++) {
        cout << name[k] << ": " << bestCase[k] << "  ";
    }
    cout << endl;
}