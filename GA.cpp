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
parent_t pool[POPULATION_SIZE]; // crossover pool
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
        cout << "----------" << endl;
#endif
    }
}

/** Roulette Wheel Selection
 * probability = fitness / totalFitness
 *
 * E.g., we have four individuals, and its probability is as below respectively:
 *  A    B   C   D
 * 0.15 0.3 0.2 0.35 -> 15% 30% 20% 35%
 *
 * randomly generate a number 'arrow' belongs to [0, 100]
 * [1, 15] -> arrow points to A
 * [16, 45] -> arrow points to B
 * [46, 65] -> arrow points to C
 * [66, 100] -> arrow points to D
 *
 * so now we could generalize a conclusion from above example
 * scope[0] belongs to [1, p1]
 * scope[1] belongs to [p1, p1+p2]
 * scope[2] belongs to [p1+p2, p1+p2+p3]
 * scope[3] belongs to [p1+p2+p3, p1+p2+p3+p4]
 */
void selectRW() {
    float probabilities[POPULATION_SIZE], scope[POPULATION_SIZE];
    int arrow, totalFitness = 0;
//TODO: 解決total fitness 為負數的情況，但不可降低punishment coefficient
    for (int i = 0; i < POPULATION_SIZE; i++) {
        totalFitness += population[i].fitness; // count total fitness
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        probabilities[i] = population[i].fitness / (float) totalFitness * 100; // calculate probabilities

        if (i == 0)
            scope[0] = probabilities[0];
        else
            scope[i] = scope[i - 1] + probabilities[i];
    }

#if DEBUG_MODE
    cout << "========== Roulette Wheel Selection ==========" << endl;
    cout << "total fitness: " << totalFitness << endl;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "probabilities[" << i << "]: " << probabilities[i] << endl;
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "scope[" << i << "]: " << scope[i] << endl;
    }
#endif

    for (int i = 0; i < POPULATION_SIZE; i++) {
        arrow = myRandom(1, 100); // arrow points to [1, 100] randomly

#if DEBUG_MODE
        cout << "arrow points at: " << arrow << endl;
#endif

        for (int j = 0; j < POPULATION_SIZE; j++) {
            if (arrow < scope[j]) { // arrow points to population[i], i.e., Select population[i]
#if DEBUG_MODE
                cout << "selected index: [" << j << "]\tfitness: " << population[j].fitness << endl;
#endif
                memcpy(&pool[i], &population[j], sizeof(parent_t));
                break; // break for loop if you already selected.
            }
        }
    }
}

// Single-Point Crossover
void crossoverSP() {
#if DEBUG_MODE
    cout << "========== Single-Point Crossover ==========" << endl;
#endif

    int pos1, pos2;

    for (int i = 0; i < POPULATION_SIZE; i += 2) { // 交配一次生出2個child, 所以+=2
        // pick 2 individuals randomly
        pos1 = myRandom(0, POPULATION_SIZE - 1);

        do {
            pos2 = myRandom(0, POPULATION_SIZE - 1);
        } while (pos1 == pos2);

        if (myRandom(0, 100) < CROSSOVER_RATE) { // do crossover
            // generate a crossover point randomly
            int crossoverPoint = myRandom(1, GENE_LENGTH - 1);

            for (int j = 0; j < crossoverPoint; j++) {
                population[i].gene[j] = pool[pos1].gene[j];
                population[i + 1].gene[j] = pool[pos2].gene[j];
            }

            for (int j = crossoverPoint; j < GENE_LENGTH; j++) {
                population[i + 1].gene[j] = pool[pos1].gene[j];
                population[i].gene[j] = pool[pos2].gene[j];
            }
            // calculate fitness after crossover done
            calcFitness(&population[i]);
            calcFitness(&population[i + 1]);

#if DEBUG_MODE
            cout << "pool[" << pos1
                 << "] and pool[" << pos2
                 << "] do crossover at crossover point: [" << crossoverPoint << ']' << endl;
            cout << "replaced population[" << i
                 << "] and population[" << i + 1
                 << "] with newborn children" << endl;
#endif
        } else { // don't crossover
            memcpy(&population[i], &pool[pos1], sizeof(parent_t));
            memcpy(&population[i + 1], &pool[pos1], sizeof(parent_t));
        }
    }

#if DEBUG_MODE
    cout << "Crossover pool: " << endl; // print crossover pool

    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "pool[" << i << "]: ";

        for (int j = 0; j < GENE_LENGTH; j++) {
            cout << setw(2) << pool[i].gene[j] << ' ';
        }
        cout << endl;
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "index: " << i
             << "\tweight: " << pool[i].weight
             << "\tvalue: " << pool[i].value
             << "\tfitness: " << pool[i].fitness << endl;
    }

    // print the state after crossover
    cout << "Population pool: " << endl;
    showState();
#endif
}

void crossoverKP() {
#if DEBUG_MODE
    cout << "========== K-Point Crossover ==========" << endl;
#endif
    //TODO: complete k-point crossover

}

void crossoverMask() {

}

/** Single-Point Mutation
 * only one bit of each individual has a chance to be a mutation point.
 */
void mutateSP() {
#if DEBUG_MODE
    cout << "========== Single-Point Mutation ==========" << endl;
#endif

    for (int i = 0; i < POPULATION_SIZE; i++) {
        if ((myRandom(0, 100)) < MUTATION_RATE) {
            int pos = myRandom(0, GENE_LENGTH - 1); // set mutating position
            population[i].gene[pos] = myRandom(0, 10);

            // calculate fitness if mutation happened
            calcFitness(&population[i]);
            // check if best gene changed
            if (population[i].fitness > bestGene.fitness) {
                memcpy(&bestGene, &population[i], sizeof(parent_t));
            }

#if DEBUG_MODE
            cout << "population[" << i
                 << "] mutated at position: [" << pos << ']' << endl;
#endif
        }
    }

#if DEBUG_MODE
    showState();
#endif
}

/** Multiple-Point Mutation
 * every single bit of each individual has a chance to be a mutation point.
 */
void mutateMP() {
#if DEBUG_MODE
    cout << "========== Multiple-Point Mutation ==========" << endl;
#endif

    for (int i = 0; i < POPULATION_SIZE; i++) {
#if DEBUG_MODE
        cout << "population[" << i
             << "] mutated at position: ";
#endif

        for (int pos = 0; pos < GENE_LENGTH; pos++) {
            if ((myRandom(0, 100)) < MUTATION_RATE) {
                population[i].gene[pos] = myRandom(0, 10);

#if DEBUG_MODE
                cout << "[" << pos << "] ";
#endif
            }
        }
#if DEBUG_MODE
        cout << endl;
#endif

        // calculate fitness after mutation done
        calcFitness(&population[i]);
        // check if best gene changed
        if (population[i].fitness > bestGene.fitness) {
            memcpy(&bestGene, &population[i], sizeof(parent_t));
        }
    }

#if DEBUG_MODE
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

    cout << "----------" << endl;
    cout << "Best gene weight: " << bestGene.weight
         << "\tvalue: " << bestGene.value
         << "\tfitness: " << bestGene.fitness << endl;
}

// Print the number of items taken
void showResult() {
    cout << "========== Result at this round ==========" << endl;
    cout << "Best case: ";

    // See how many each item is taken
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
