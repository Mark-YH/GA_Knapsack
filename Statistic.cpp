//
// Created by Mark Hsu on 2019-07-08.
//
#include "GA.h"
#include "Statistic.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

int cntOverweight = 0; // count overweight times
int totalValue = 0;
int bestValue = 0;
int bestWeight = 0;
int bestFitness = 0;
int bestCase[10];
int cntBestCase = 0;

void prtLine() {
    for (int i = 0; i < 78; i++) {
        if (i % 11 == 0)
            cout << '+';
        else
            cout << '-';
    }
    cout << endl;
}

bool isOverweight(int w) {
    return w > KNAPSACK_SIZE;
}

// statistic of result of total rounds
void statistic(int *best) {
    // check if it is overweight
    if (isOverweight(best[10])) {
        cntOverweight++;
#if DEBUG_MODE
        cout << "Overweight!!!" << endl;
#endif
    } else if (best[12] == 620) {
        cntBestCase++;
    }

    totalValue += best[11];

    // check if it needs to update best fitness
    if (best[12] > bestFitness) {
        bestWeight = best[10];
        bestValue = best[11];
        bestFitness = best[12];
        memcpy(bestCase, best, sizeof(int) * GENE_LENGTH);
    }
}

void finalResult() {
    cout << "==================== STATISTIC ====================\n";
#if SELECTION_TYPE == 0
    cout << "Selection type: Tournament" << endl;
#else
    cout << "Selection type: Roulette Wheel" << endl;
#endif

#if CROSSOVER_TYPE == 0
    cout << "Crossover type: Single-Point" << endl;
#elif CROSSOVER_TYPE == 1
    cout << "Crossover type: K-Point" << endl;
#else
    cout << "Crossover type: Mask" << endl;
#endif

#if MUTATION_TYPE == 0
    cout << "Mutation type: Single-Point" << endl;
#else
    cout << "Mutation type: Multiple-Point" << endl;
#endif

    cout << "Round: " << ROUND << endl;
    cout << "Generation: " << GENERATION << endl;
    cout << "Population: " << POPULATION_SIZE << endl;
    cout << "Crossover rate: " << CROSSOVER_RATE << "%" << endl;
    cout << "Mutation rate: " << MUTATION_RATE << "%" << endl << endl;

    prtLine();

    cout << '|'
         << setw(10) << "Overweight" << '|'
         << setw(10) << "Overweight" << '|'
         << setw(10) << "Best case" << '|'
         << setw(10) << "Avg.   " << '|'
         << setw(10) << "Best   " << '|'
         << setw(10) << "Best   " << '|'
         << setw(10) << "Best case" << '|' << endl;

    cout << '|'
         << setw(10) << "count  " << '|'
         << setw(10) << "rate   " << '|'
         << setw(10) << "weight  " << '|'
         << setw(10) << "value  " << '|'
         << setw(10) << "value  " << '|'
         << setw(10) << "fitness " << '|'
         << setw(10) << "count  " << '|' << endl;

    prtLine();

    cout << '|'
         << setw(9) << cntOverweight << " |"
         << setw(7) << fixed << setprecision(2) << cntOverweight / (float) ROUND * 100
         << setw(2) << " %" << " |"
         << setw(9) << bestWeight << " |"
         << setw(9) << setprecision(2) << totalValue / (float) ROUND << " |"
         << setw(9) << bestValue << " |"
         << setw(9) << bestFitness << " |"
         << setw(9) << cntBestCase << " |" << endl;

    prtLine();

    for (int k = 0; k < 10; k++) {
        cout << name[k] << ": " << bestCase[k] << "  ";
    }
    cout << endl;
}
