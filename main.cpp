#include "GA.h"
#include "Statistic.h"
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <chrono>
#include <sys/wait.h>

using namespace std;

int main() {
    // Safety check
#if K_POINT_CROSSOVER > GENE_LENGTH - 1
    cout << "ERROR: K_POINT_CROSSOVER is out of bound.";
    return -1;
#endif

    auto start = chrono::steady_clock::now();// count execution time

    pid_t pid = 1;
    int fd[2];
    int *fdWrite = &fd[1];
    int *fdRead = &fd[0];

    if (pipe(fd) == -1) {
        cout << "error occurred when pipe()" << endl;
        return -1;
    }

    for (int i = 0; i < ROUND; i++) {
        if (pid > 0) {
            pid = fork();
        } else {
            break;
        }
    }

    if (pid < 0) {
        cout << "error occurred when fork()" << endl;
        return -1;
    }

    if (pid == 0) { // child process
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

#if DEBUG_MODE
        cout << "in child process: ";
        for (int i = 0; i < (RESULT_LENGTH); i++) {
            cout << rs[i] << ' ';
        }
        cout << endl;
#endif

        close(*fdRead);
        write(*fdWrite, getBestGeneArr(), sizeof(int) * (RESULT_LENGTH));
        close(*fdWrite);

    } else if (pid > 0) {
        close(*fdWrite);

        for (int i = 0; i < ROUND; i++) {
            wait(nullptr);

            int result[RESULT_LENGTH];
            read(*fdRead, &result, sizeof(int) * RESULT_LENGTH);

#if DEBUG_MODE
            cout << "in main process: ";
            for (int i = 0; i < (RESULT_LENGTH); i++) {
                cout << result[i] << ' ';
            }
#endif
            statistic(result);
        }

        close(*fdRead);

        finalResult();

        auto end = chrono::steady_clock::now();
        cout << "Time taken: " << chrono::duration<double>(end - start).count() << " s" << endl;
    }
    return 0;
}
