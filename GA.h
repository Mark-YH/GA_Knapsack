//
// Created by Mark Hsu on 2019-07-08.
//

#define GENERATION 1 // number of generation
#define POPULATION_SIZE 4 // number of population
#define MUTATION_RATE 50 // percentage, e.g., 10 stands for  10%
#define CROSSOVER_RATE 100 // percentage, e.g., 10 stands for  10%
#define ALPHA 18 // punishment coefficient
#define KNAPSACK_SIZE 275 // number of max knapsack size
#define GENE_LENGTH 10 // length of gene

/**
 * DEBUG_MODE 1: print debugging information
 *            0: do not print debugging information
 *
 * EACH_ROUND_RESULT 1: print each round result respectively
 *                   0: do not print each round result respectively
 */
#define DEBUG_MODE 1
#define EACH_ROUND_RESULT 1

/**
 * SELECTION_TYPE 0: Tournament Selection
 *                1: Roulette Wheel Selection
 *
 * CROSSOVER_TYPE 0: Single-Point Crossover
 *                1: K-Point Crossover
 *                2: Mask Crossover
 *
 * K_POINT_CROSSOVER k: set k-point crossover, it should set between 2 and (GENE_LENGTH - 1)
 *
 * MUTATION_TYPE 0: Single-Point Mutation
 *               1: Multiple-Point Mutation
 */
#define SELECTION_TYPE 0
#define CROSSOVER_TYPE 1
#define K_POINT_CROSSOVER 8
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

void crossoverKP();

void crossoverMask();

void mutateSP();

void mutateMP();

void showResult();

parent_t *getBestGene();

