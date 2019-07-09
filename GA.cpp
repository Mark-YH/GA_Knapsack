//
// Created by Mark Hsu on 2019-07-08.
//
#include "GA.h"
#include <iostream>
#include <stdlib.h>
#include <random>

using namespace std;

//int valBest1, valBest2, iBest1, iBest2;
int cntResult[10];
parent_t population[POPULATION_SIZE];
parent_t pool[POPULATION_SIZE];
parent_t bestGene;

/*
 * The first dimension of gen stands for 4 chromosomes.
 *
 * The second dimension of array of gen:
 *  [0] -  [9] for 'A' which weight = 1 and value = 6
 * [10] - [19] for 'B' which weight = 2 and value = 7
 * ... and so on
 * [90] - [99] for 'J' which weight = 10 and value = 15
 *
 * [100] for weight.
 * [101] for fitness.
 */


void testBestCase() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < 68; j++) {
            population[i].gene[j] = 1;
        }
        for (int k = 68; k < 100; k++) {
            if (k == 80) {
                population[i].gene[k] = 1;
            } else {
                population[i].gene[k] = 0;
            }
        }
        calcFitness(&population[i]);
    }
    showState();
}

int myRandom(int start, int end) {
    random_device rd;
    default_random_engine gen = std::default_random_engine(rd());
    uniform_int_distribution<int> dis(start, end);
    return dis(gen);
}

void init() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < GENE_LENGTH; j++) {
            population[i].gene[j] = myRandom(0, 1);
            calcFitness(&population[i]);
        }
        if (i == 0) {
            memcpy(&bestGene, &population[i], sizeof(parent_t));
        } else if (population[i].fitness > bestGene.fitness) {
            memcpy(&bestGene, &population[i], sizeof(parent_t));
        }
    }
    if (DEBUG_MODE) {
        cout << "========== Initialization ==========" << endl;
        showState();
    }
}

void calcFitness(parent_t *x) {
    int w = 0;// Total weight
    int v = 0;// Total value

    for (int j = 0; j < GENE_LENGTH; j++) {
        if (j < 10) {
            w += x->gene[j] * weight[0];
            v += x->gene[j] * value[0];
        } else if (j < 20) {
            w += x->gene[j] * weight[1];
            v += x->gene[j] * value[1];
        } else if (j < 30) {
            w += x->gene[j] * weight[2];
            v += x->gene[j] * value[2];
        } else if (j < 40) {
            w += x->gene[j] * weight[3];
            v += x->gene[j] * value[3];
        } else if (j < 50) {
            w += x->gene[j] * weight[4];
            v += x->gene[j] * value[4];
        } else if (j < 60) {
            w += x->gene[j] * weight[5];
            v += x->gene[j] * value[5];
        } else if (j < 70) {
            w += x->gene[j] * weight[6];
            v += x->gene[j] * value[6];
        } else if (j < 80) {
            w += x->gene[j] * weight[7];
            v += x->gene[j] * value[7];
        } else if (j < 90) {
            w += x->gene[j] * weight[8];
            v += x->gene[j] * value[8];
        } else {
            w += x->gene[j] * weight[9];
            v += x->gene[j] * value[9];
        }

        x->weight = w;
        x->value = v;
        if (w <= KNAPSACK_SIZE) {
            x->fitness = v;
        } else {
            x->fitness = v - ALPHA * (w - KNAPSACK_SIZE);
        }
    }
}

void selectTournament() {
    if (DEBUG_MODE)
        cout << "========== Tournament Selection ==========" << endl;

    int pos1, pos2;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        // pick 2 individuals randomly
        pos1 = myRandom(0, POPULATION_SIZE - 1);
        do {
            pos2 = myRandom(0, POPULATION_SIZE - 1);
        } while (pos1 == pos2);

        if (population[pos1].fitness > population[pos2].fitness) {
            memcpy(&pool[i], &population[pos1], sizeof(parent_t));
        } else {
            memcpy(&pool[i], &population[pos2], sizeof(parent_t));
        }

        if (DEBUG_MODE) {
            cout << "picked genes: index: [" << pos1 << "] and [" << pos2 << "]" << endl;
            if (population[pos1].fitness > population[pos2].fitness) {
                cout << "selected index: [" << pos1 << "]\tfitness: " << population[pos1].fitness << endl;
            } else {
                cout << "selected index: " << pos2 << "\tfitness: " << population[pos2].fitness << endl;
            }
        }
    }
}

void selectRW() {

}

// Single-Point Crossover
void crossoverSP() {
    if (DEBUG_MODE) {
        cout << "========== Single-Point Crossover ==========" << endl;

        // See crossover pool
        cout << "Crossover pool: " << endl;
        for (int i = 0; i < POPULATION_SIZE; i++) {
            cout << "index: " << i << "\tweight: " << pool[i].weight << "\tvalue: " << pool[i].value
                 << "\tfitness: " << pool[i].fitness << endl;
        }
    }

    int pos1, pos2;

    for (int i = 0; i < POPULATION_SIZE; i += 2) { // 交配一次生出2個child, 所以+=2
        // pick 2 individuals randomly
        pos1 = myRandom(0, POPULATION_SIZE - 1);
        do {
            pos2 = myRandom(0, POPULATION_SIZE - 1);
        } while (pos1 == pos2);

        if (myRandom(1, 100) > CROSSOVER_RATE) { // do crossover
            int crossoverPoint = myRandom(1, GENE_LENGTH - 1);

            for (int j = 0; j < crossoverPoint; j++) {
                population[i].gene[j] = pool[pos1].gene[j];
                population[i + 1].gene[j] = pool[pos2].gene[j];
            }
            for (int j = crossoverPoint; j < GENE_LENGTH; j++) {
                population[i + 1].gene[j] = pool[pos1].gene[j];
                population[i].gene[j] = pool[pos2].gene[j];
            }
            calcFitness(&population[i]);
            calcFitness(&population[i + 1]);
        } else { // don't crossover
            memcpy(&population[i], &pool[pos1], sizeof(parent_t));
            memcpy(&population[i + 1], &pool[pos1], sizeof(parent_t));
        }
    }
}

void crossoverMP() {

}

void crossoverMask() {

}

// Single-Point Mutation
void mutateSP() {
//    if (DEBUG_MODE)
//        cout << "========== Single-Point Mutation ==========" << endl;
//    int i = myRandom(POPULATION_SIZE); // choose chromosome
//    int j = myRandom(10); // choose item type from 'A' to 'J'. e.g., 0 for 'A', ..., 9 for 'J'
//
//    for (int k = (j * 10); k < 10; k++) {
//        if ((myRandom(100)) < MUTATION_RATE) {
//            gen[i][j] = !gen[i][j];
//        }
//    }


//    // need to update fitness and bestGene after mutation
//    calcFitness(&population[i]);
//    if (population[i].fitness > bestGene.fitness) {
//        memcpy(&bestGene, &population[i], sizeof(parent_t));
//    }

}

// Multiple-Point Mutation
void mutateMP() {
    int i = myRandom(0, POPULATION_SIZE - 1);

    for (int j = 0; j < GENE_LENGTH; j++) {
        if ((myRandom(1, GENE_LENGTH)) < MUTATION_RATE) {
            population[i].gene[j] = 1 - population[i].gene[j];
        }
    }
    calcFitness(&population[i]);
    if (population[i].fitness > bestGene.fitness) {
        memcpy(&bestGene, &population[i], sizeof(parent_t));
    }

    if (DEBUG_MODE) {
        cout << "========== Multiple-Point Mutation ==========" << endl;
        showState();
    }
}

// Print each chromosome state
void showState() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "population[" << i << "]: ";
        for (int j = 0; j < GENE_LENGTH; j++) {
            cout << population[i].gene[j];
            if ((j + 1) % 10 == 0) {
                cout << ' ';
            }
        }
        cout << endl;
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "index: " << i << "\tweight: " << population[i].weight << "\tvalue: " << population[i].value
             << "\tfitness: " << population[i].fitness << endl;
    }
    cout << "Best gene weight: " << bestGene.weight
         << "\tvalue: " << bestGene.value
         << "\tfitness: " << bestGene.fitness << endl;
}

// Print the number of items taken
void showResult() {
    cout << "========== Result at this round ==========" << endl;

    int *p = getResultCnt();

    // See how many each item is taken
    cout << "Best case: ";
    for (int i = 0; i < 10; i++) {
        cout << name[i] << ": " << *(p + i) << "  ";
    }
    cout << endl;

    cout << "bestGene weight: " << bestGene.weight
         << "\t value: " << bestGene.value
         << "\t fitness: " << bestGene.fitness << endl;

}

int *getResultCnt() {
    for (int i = 0; i < 10; i++) {
        cntResult[i] = 0;
    }

    for (int j = 0; j < GENE_LENGTH; j++) {
        if (bestGene.gene[j] > 0) {
            if (j < 10) {
                cntResult[0]++;
            } else if (j < 20) {
                cntResult[1]++;
            } else if (j < 30) {
                cntResult[2]++;
            } else if (j < 40) {
                cntResult[3]++;
            } else if (j < 50) {
                cntResult[4]++;
            } else if (j < 60) {
                cntResult[5]++;
            } else if (j < 70) {
                cntResult[6]++;
            } else if (j < 80) {
                cntResult[7]++;
            } else if (j < 90) {
                cntResult[8]++;
            } else {
                cntResult[9]++;
            }
        }
    }
    return cntResult;
}

parent_t *getResult() {
//    static int rs[2];
//    rs[0] = gen[iBest1][100];
//    rs[1] = gen[iBest1][101];
    return &bestGene;
}
