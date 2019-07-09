//
// Created by Mark Hsu on 2019-07-08.
//

 #define GENERATION 4000 // number of generation
#define POPULATION_SIZE 4 // number of population
#define GENE_LENGTH 10 // length of gene
#define MUTATION_RATE 50 // percentage, e.g., 10 stands for  10%
#define CROSSOVER_RATE 50 // percentage, e.g., 10 stands for  10%
#define KNAPSACK_SIZE 275 // number of max knapsack size
#define ALPHA 18 // punishment coefficient
#define DEBUG_MODE 0 // print debugging information? 1 for yes, 0 for no
#define EACH_ROUND_RESULT 0 // print each round result respectively? 1 for yes, 0 for no

/**
 * SELECTION_TYPE 0: Tournament Selection
 * SELECTION_TYPE 1: Roulette Wheel Selection
 *
 * CROSSOVER_TYPE 0: Single-Point Crossover
 * CROSSOVER_TYPE 1: K-Point Crossover
 * CROSSOVER_TYPE 2: Mask Crossover
 *
 * MUTATION_TYPE 0: Single-Point Mutation
 * MUTATION_TYPE 1: Multiple-Point Mutation
 */
#define SELECTION_TYPE 0
#define CROSSOVER_TYPE 0
#define MUTATION_TYPE 1

typedef struct tag_parent_t {
    int gene[GENE_LENGTH];
    int weight;
    int value;
    int fitness;
} parent_t;

const char name[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
const int weight[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int value[10] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void init();

void calcFitness(parent_t *x);

void selectTournament();

void selectRW();

void crossoverSP();

void crossoverMP();

void crossoverMask();

void mutateSP();

void mutateMP();

void showState();

void showResult();

parent_t *getResult();

int myRandom(int, int);