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
            fitness();


            // Choose the method you want.
            selectTournament();
//            selectRW();


            crossoverSP();
//            crossoverMP();
//            crossoverMask();


//            mutateSP();
            mutateMP();
        }
        if (DEBUG_MODE)
            showResult();
        statistic();
    }
    finalResult();

    printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
