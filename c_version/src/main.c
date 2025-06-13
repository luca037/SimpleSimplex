#include <stdio.h>
#include <string.h>

#include "../include/fraction.h"
#include "../include/utils.h"
#include "../include/simple_simplex.h"


// Function that tests the two phases simplex.
void two_phase_tester(void);

// Function that tests the dual simplex.
void dual_simplex_tester(void);


int main(int argc, char *argv[]) {
    char *num_fn = argv[1];
    char *den_fn = argv[2];
    char *mode = argv[5];

    int rows = atoi(argv[3]);
    int cols = atoi(argv[4]);

    Tableau tab;
    int status = load_tableau(num_fn, den_fn, rows, cols, &tab);
    if (!status) {
        printf("Status ok.\n");
        pretty_print_tableau(&tab);
    }

    // Check mode and lauch solver.
    if (!strcmp("TPS", mode)) { // Two Phase Simplex.

        // Allocate memory for the basis.
        size_t *basis = (size_t*) malloc(tab.m * sizeof(size_t));
        if (basis == NULL) {
            fprintf(stderr, "Error - Not enough memory to allocate the basis.\n");
            free_and_null((char**) &basis);
            return 1;
        }

        printf("### Starting phase one... ###\n");
        int status = phase_one(&tab, basis);
        if (status == FEASIBLE) {
            printf("\n### Problem is feasible. Starting phase two... ###\n");
            simplex(&tab, basis);
        }
        
        // Free memory.
        free_and_null((char**) &basis);

    } else if (!strcmp("DS", mode)) { // Dual simplex.
    } else {
        fprintf(stderr, "Error - Mode specified not defined.");
    }

    return 0;
}


void two_phase_tester(void) {
    // Define the tableau.
    Tableau tab = {
        3, // # cols 
        2, // # rows
        NULL
    };

    // Define the original tableau.
    Fraction datas[] = {
        {0, 1}, {1, 1}, {1, 1}, {10, 1}, 
        {2, 1}, {0, 1}, {1, 1}, {4, 1},
        {2, 1}, {-2, 1}, {1, 1}, {-6, 1},
    };
    tab.data = datas;

    // Allocate memory for the basis.
    size_t *basis = (size_t*) malloc(tab.m * sizeof(size_t));
    if (basis == NULL) {
        fprintf(stderr, "Error - Not enough memory to allocate the basis.\n");
        goto TERMINATE;
    }

    printf("### Starting phase one... ###\n");
    int status = phase_one(&tab, basis);
    if (status == FEASIBLE) {
        printf("\n### Problem is feasible. Starting phase two... ###\n");
        simplex(&tab, basis);
    }

TERMINATE:
    // Free up allocated memory.
    free_and_null((char**) &basis);
}

void dual_simplex_tester(void) {
    // Define the tableau.
    Tableau tab = {
        5, // # cols
        2, // # rows
        NULL
    };

    // Define the tableau.
    Fraction datas[] = {
        {0, 1}, {3, 1}, {4, 1}, {5, 1}, {0, 1}, {0, 1},
        {-6, 1}, {-2, 1}, {-2, 1}, {-1, 1}, {1, 1}, {0, 1},
        {-5, 1}, {-1, 1}, {-2, 1}, {-3, 1}, {0, 1}, {1, 1},
    };
    tab.data = datas;

    // Define the basis.
    size_t basis[] = {4, 5};
    dual_simplex(&tab, basis);
}
