#include <stdio.h>

#include "../include/fraction.h"
#include "../include/utils.h"
#include "../include/simple_simplex.h"


// Function that tests the two phases simplex.
void two_phase_tester(void);

// Function that tests the dual simplex.
void dual_simplex_tester(void);


int main(void) {

    dual_simplex_tester();

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
