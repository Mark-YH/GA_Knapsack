#include "GA.h"
#include "Statistic.h"
#include <iostream>
#include <time.h>

using namespace std;

int main() {
    clock_t tStart = clock();

    for (int i = 0; i < ROUND; i++) {
        cout << "Round " << (i + 1) << "/" << ROUND << endl;

        init();

        for (int j = 0; j < GENERATION; j++) {
            // Choose the method you want.
 #if SELECTION_TYPE == 0
            selectTournament();
#else
            selectRW();
#endif
#if CROSSOVER_TYPE == 0
            crossoverSP();
#elif CROSSOVER_TYPE == 1
            crossoverMP();
#else
            crossoverMask();
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

    printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
