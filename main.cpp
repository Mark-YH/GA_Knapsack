#include "GA.h"
#include "Statistic.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

using namespace std;

int main() {
    clock_t tStart = clock(); // count execution time

    pid_t pid = 0;

    for (int i = 0; i < 5; i++) {
        if (pid == 0) {
            tStart = clock(); // count execution time
            pid = fork();
        } else
            break;
    }

    if (pid == 0) {
        cout << "parent pid: " << pid << endl;
    } else {
        cout << "child pid: " << pid << endl;
    }

    // Safety check
#if K_POINT_CROSSOVER > GENE_LENGTH - 1
    cout << "ERROR: K_POINT_CROSSOVER is out of bound.";
    return 0;
#endif

    for (int i = 0; i < ROUND; i++) {
        cout << "Round " << (i + 1) << "/" << ROUND << endl;
        init();

        for (int j = 0; j < GENERATION; j++) {
#if SELECTION_TYPE == 0
            selectTournament();
#else
            selectRW();
#endif
#if CROSSOVER_TYPE == 0
            crossoverSP();
#else
            crossoverKP();
#endif
#if MUTATION_TYPE == 0
            mutateSP();
#else
            mutateMP();
#endif
        }
#if EACH_ROUND_RESULT
        showResult();
#endif
        statistic();
    }
    finalResult();

    wait(nullptr);
    if (pid > 0)
        exit(0);
    printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC); // print execution time
    return 0;
}
