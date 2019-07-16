//
// Created by Mark Hsu on 2019-07-08.
//

#include "Statistic.h"
#include "GA.h"
#include <iostream>
#include <iomanip>

using namespace std;

int cntOverweight = 0; // count overweight times
int totalValue = 0;
int bestValue = 0;
int bestWeight = 0;
int bestFitness = 0;
int bestCase[10];
int cntBestCase = 0;

bool isOverweight(int w) {
    return w > KNAPSACK_SIZE;
}

// statistic of result of total rounds
void statistic() {
    parent_t *best;
    best = getBestGene();

    // check if it is overweight
    if (isOverweight(best->weight)) {
        cntOverweight++;
#if DEBUG_MODE
        cout << "Overweight!!!" << endl;
#endif
    } else if (best->fitness == 620) {
        cntBestCase++;
    }

    totalValue += best->value;

    // check if it needs to update best fitness
    if (best->fitness > bestFitness) {
        bestWeight = best->weight;
        bestValue = best->value;
        bestFitness = best->fitness;
        memcpy(bestCase, best->gene, sizeof(bestCase));
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

    cout << setw(17) << "Overweight count" << " |"
         << setw(16) << "Overweight rate" << " |"
         << setw(17) << "Best case weight" << " |"
         << setw(11) << "Avg. value" << " |"
         << setw(11) << "Best value" << " |"
         << setw(13) << "Best fitness" << " |"
         << setw(15) << "Best case count" << endl;

    cout << setw(17) << cntOverweight << " |"
         << setw(14) << fixed << setprecision(2) << cntOverweight / (float) ROUND * 100
         << setw(2) << " %" << " |"
         << setw(17) << bestWeight << " |"
         << setw(11) << setprecision(2) << totalValue / (float) ROUND << " |"
         << setw(11) << bestValue << " |"
         << setw(13) << bestFitness << " |"
         << setw(15) << cntBestCase << endl;

    cout << "Best case: ";
    for (int k = 0; k < 10; k++) {
        cout << name[k] << ": " << bestCase[k] << "  ";
    }
    cout << endl;

    std::cout << "\007";
}