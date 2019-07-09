//
// Created by Mark Hsu on 2019-07-08.
//
#include "GA.h"
#include <iostream>
#include <stdlib.h>
#include <random>
#include <iomanip>

using namespace std;

parent_t population[POPULATION_SIZE];
parent_t pool[POPULATION_SIZE];
parent_t bestGene;

int myRandom(int start, int end) {
    random_device rd;
    default_random_engine gen = std::default_random_engine(rd());
    uniform_int_distribution<int> dis(start, end);
    return dis(gen);
}

void init() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < GENE_LENGTH; j++) {
            population[i].gene[j] = myRandom(0, 10);
        }
        calcFitness(&population[i]);

        if (i == 0) {
            memcpy(&bestGene, &population[i], sizeof(parent_t));
        } else if (population[i].fitness > bestGene.fitness) {
            memcpy(&bestGene, &population[i], sizeof(parent_t));
        }
    }
#if DEBUG_MODE
    cout << "========== Initialization ==========" << endl;
    showState();
#endif
}

void calcFitness(parent_t *x) {
    x->weight = 0;
    x->value = 0;
    for (int j = 0; j < GENE_LENGTH; j++) {
        x->weight += x->gene[j] * weight[j];
        x->value += x->gene[j] * value[j];
    }
    if (x->weight <= KNAPSACK_SIZE) {
        x->fitness = x->value;
    } else {
        x->fitness = x->value - ALPHA * (x->weight - KNAPSACK_SIZE);
    }
}

void selectTournament() {
#if DEBUG_MODE
    cout << "========== Tournament Selection ==========" << endl;
#endif
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
#if DEBUG_MODE
        cout << "picked genes: index: [" << pos1 << "] and [" << pos2 << "]" << endl;
        if (population[pos1].fitness > population[pos2].fitness) {
            cout << "selected index: [" << pos1 << "]\tfitness: " << population[pos1].fitness << endl;
        } else {
            cout << "selected index: [" << pos2 << "]\tfitness: " << population[pos2].fitness << endl;
        }
#endif
    }
}

void selectRW() {

}

// Single-Point Crossover
void crossoverSP() {
#if DEBUG_MODE
    cout << "========== Single-Point Crossover ==========" << endl;

    // See crossover pool
    cout << "Crossover pool: " << endl;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "index: " << i << "\tweight: " << pool[i].weight << "\tvalue: " << pool[i].value
             << "\tfitness: " << pool[i].fitness << endl;
    }
#endif

    int pos1, pos2;

    for (int i = 0; i < POPULATION_SIZE; i += 2) { // 交配一次生出2個child, 所以+=2
        // pick 2 individuals randomly
        pos1 = myRandom(0, POPULATION_SIZE - 1);
        do {
            pos2 = myRandom(0, POPULATION_SIZE - 1);
        } while (pos1 == pos2);

        if (myRandom(0, 100) > CROSSOVER_RATE) { // do crossover
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
    for (int i = 0; i < POPULATION_SIZE; i++) {
        int pos = myRandom(0, GENE_LENGTH - 1); // set mutating position
        if ((myRandom(0, 100)) < MUTATION_RATE) {
            population[i].gene[pos] = myRandom(0, 10);
        }

        calcFitness(&population[i]);

        if (population[i].fitness > bestGene.fitness) {
            memcpy(&bestGene, &population[i], sizeof(parent_t));
        }
    }
#if DEBUG_MODE
    cout << "========== Multiple-Point Mutation ==========" << endl;
    showState();
#endif
}

// Print each chromosome state
void showState() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "population[" << i << "]: ";
        for (int j = 0; j < GENE_LENGTH; j++) {
            cout << setw(2) << population[i].gene[j] << ' ';
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

    // See how many each item is taken
    cout << "Best case: ";
    for (int i = 0; i < GENE_LENGTH; i++) {
        cout << name[i] << ": " << bestGene.gene[i] << "  ";
    }
    cout << endl;

    cout << "bestGene weight: " << bestGene.weight
         << "\t value: " << bestGene.value
         << "\t fitness: " << bestGene.fitness << endl;
}

parent_t *getResult() {
    return &bestGene;
}
