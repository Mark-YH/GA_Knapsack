//
// Created by Mark Hsu on 2019-07-08.
//
#include "GA.h"
#include <iostream>
#include <stdlib.h>
#include <random>

using namespace std;

int gen[POPULATION_NUM][102];
int valBest1, valBest2, iBest1, iBest2;
int cntResult[10];
parent_t population[POPULATION_NUM];

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

int myRandom(int range) {
    random_device rd;
    default_random_engine gen = std::default_random_engine(rd());
    uniform_int_distribution<int> dis(0, range - 1);
    return dis(gen);
}

void init() {
    if (DEBUG_MODE)
        cout << "========== Initialization ==========" << endl;
    for (int i = 0; i < POPULATION_NUM; i++) {
        for (int j = 0; j < 100; j++) {
            gen[i][j] = myRandom(2);
        }
    }
}

void fitness() {
    if (DEBUG_MODE) {
        cout << "========== Fitness ==========" << endl;
        showState();
    }
    int w;// Total weight
    int v;// Total value
    for (int i = 0; i < POPULATION_NUM; i++) {
        w = 0;
        v = 0;
        for (int j = 0; j < 100; j++) {
            if (j < 10) {
                w += gen[i][j] * weight[0];
                v += gen[i][j] * value[0];
            } else if (j < 20) {
                w += gen[i][j] * weight[1];
                v += gen[i][j] * value[1];
            } else if (j < 30) {
                w += gen[i][j] * weight[2];
                v += gen[i][j] * value[2];
            } else if (j < 40) {
                w += gen[i][j] * weight[3];
                v += gen[i][j] * value[3];
            } else if (j < 50) {
                w += gen[i][j] * weight[4];
                v += gen[i][j] * value[4];
            } else if (j < 60) {
                w += gen[i][j] * weight[5];
                v += gen[i][j] * value[5];
            } else if (j < 70) {
                w += gen[i][j] * weight[6];
                v += gen[i][j] * value[6];
            } else if (j < 80) {
                w += gen[i][j] * weight[7];
                v += gen[i][j] * value[7];
            } else if (j < 90) {
                w += gen[i][j] * weight[8];
                v += gen[i][j] * value[8];
            } else {
                w += gen[i][j] * weight[9];
                v += gen[i][j] * value[9];
            }
        }

        gen[i][100] = w;
        if (w <= KNAPSACK_SIZE) {
            gen[i][101] = v;
        } else {
            gen[i][101] = v - ALPHA * (w - KNAPSACK_SIZE);
        }
        if (DEBUG_MODE)
            cout << "index: " << i << "\tweight: " << w << "\tvalue: " << v
                 << "\tfitness: " << gen[i][101] << endl;
    }
}

void selectTournament() {
    if (DEBUG_MODE)
        cout << "========== Tournament Selection ==========" << endl;

    valBest1 = 0;
    valBest2 = 0;
    iBest1 = 0;
    iBest2 = 0;

    for (int i = 0; i < POPULATION_NUM; i++) {
        if (valBest1 < valBest2) {
            if (gen[i][101] > valBest1) {
                iBest1 = i;
                valBest1 = gen[i][101];
            }
        } else {
            if (gen[i][101] > valBest2) {
                iBest2 = i;
                valBest2 = gen[i][101];
            }
        }
    }
    if (valBest2 > valBest1) {
        int tmpVal = valBest1;
        int tmpIndex = iBest1;
        valBest1 = valBest2;
        iBest1 = iBest2;
        valBest2 = tmpVal;
        iBest2 = tmpIndex;
    }
    if (DEBUG_MODE)
        cout << "selected: " << endl
             << "\tgen[" << iBest1 << "]: " << valBest1 << endl
             << "\tgen[" << iBest2 << "]: " << valBest2 << endl;
}

void selectRW() {

}

// Single-Point Crossover
void crossoverSP() {
    if (DEBUG_MODE)
        cout << "========== Single-Point Crossover ==========" << endl;

    int temp[2][100];

    // temp
    for (int i = 0; i < 100; i++) {
        if (i < 50) {
            temp[0][i] = gen[iBest1][i];
            temp[1][i] = gen[iBest2][i];
        } else {
            temp[0][i] = gen[iBest2][i];
            temp[1][i] = gen[iBest1][i];
        }
    }

    // replace
    int i = 0;
    for (int j = 0; j < POPULATION_NUM; j++) {
        if (j == iBest1 || j == iBest2) {
            // do nothing. parent chromosome won't be replaced
        } else {
            // replace eliminated chromosome with child chromosome
            for (int k = 0; k < 100; k++) {
                gen[j][k] = temp[i][k];
            }
            i++;
        }
    }
}

void crossoverMP() {

}

void crossoverMask() {

}

// Single-Point Mutation
void mutateSP() {
    if (DEBUG_MODE)
        cout << "========== Single-Point Mutation ==========" << endl;
    int i = myRandom(POPULATION_NUM); // choose chromosome
    int j = myRandom(10); // choose item type from 'A' to 'J'. e.g., 0 for 'A', ..., 9 for 'J'

    for (int k = (j * 10); k < 10; k++) {
        if ((myRandom(100)) < MUTATION_RATE) {
            gen[i][j] = !gen[i][j];
        }
    }
}

// Multiple-Point Mutation
void mutateMP() {
    if (DEBUG_MODE)
        cout << "========== Multiple-Point Mutation ==========" << endl;
    int i;
    do {
        i = myRandom(POPULATION_NUM);
    } while (i == iBest1 || i == iBest2);

    for (int j = 0; j < 100; j++) {
        if ((myRandom(100)) < MUTATION_RATE) {
            gen[i][j] = !gen[i][j];
        }
    }
}

// Print each chromosome state
void showState() {
    for (int i = 0; i < POPULATION_NUM; i++) {
        cout << "gen[" << i << "]: ";
        for (int j = 0; j < 100; j++) {
            cout << gen[i][j];
            if ((j + 1) % 10 == 0) {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

// Print the number of items taken
void showResult() {
    if (DEBUG_MODE)
        cout << "========== Result at this round ==========" << endl;

    int *p;
    p = getResultCnt();

    // See how many each item is taken
    cout << "Best case: ";
    for (int k = 0; k < 10; k++) {
        cout << name[k] << ": " << *(p + k) << "  ";
    }
    cout << endl;

    cout << "gen[" << iBest1 << "]: " << valBest1 << endl;
}

int *getResultCnt() {
    for (int i = 0; i < 10; i++) {
        cntResult[i] = 0;
    }

    for (int j = 0; j < 100; j++) {
        if (j < 10) {
            if (gen[iBest1][j] > 0) {
                cntResult[0]++;
            }
        } else if (j < 20) {
            if (gen[iBest1][j] > 0) {
                cntResult[1]++;
            }
        } else if (j < 30) {
            if (gen[iBest1][j] > 0) {
                cntResult[2]++;
            }
        } else if (j < 40) {
            if (gen[iBest1][j] > 0) {
                cntResult[3]++;
            }
        } else if (j < 50) {
            if (gen[iBest1][j] > 0) {
                cntResult[4]++;
            }
        } else if (j < 60) {
            if (gen[iBest1][j] > 0) {
                cntResult[5]++;
            }
        } else if (j < 70) {
            if (gen[iBest1][j] > 0) {
                cntResult[6]++;
            }
        } else if (j < 80) {
            if (gen[iBest1][j] > 0) {
                cntResult[7]++;
            }
        } else if (j < 90) {
            if (gen[iBest1][j] > 0) {
                cntResult[8]++;
            }
        } else {
            if (gen[iBest1][j] > 0) {
                cntResult[9]++;
            }
        }
    }
    return cntResult;
}

int *getResult() {
    static int rs[2];
    rs[0] = gen[iBest1][100];
    rs[1] = gen[iBest1][101];
    return rs;
}
