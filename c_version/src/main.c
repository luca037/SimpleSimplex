#include <stdio.h>

#include "../include/fraction.h"
#include "../include/utils.h"
#include "../include/simple_simplex.h"

// FIXME: write a tester function.
int main(void) {
    
    // Define the tableau.
    Tableau tab;
    tab.n = 5;
    tab.m = 2;

    //const size_t sz = (tab.n + 1) * (tab.m + 1);

    // Define the entries of the tableau (p.47).
    Fraction tab_data[] = {
        //{0, 1}, {1, 1}, {1, 1}, {2, 1}, {4, 1},
        //{1, 1}, {0, 1}, {2, 1}, {0, 1}, {-3, 1},
        //{0, 1}, {1, 1}, {0, 1}, {0, 1}, {-1, 1},
        //{1, 1}, {-1, 1}, {0, 1}, {1, 1}, {0, 1},
        //{0, 1}, {1, 1}, {1, 1}, {10, 1}, 
        //{2, 1}, {0, 1}, {1, 1}, {4, 1},
        //{2, 1}, {-2, 1}, {1, 1}, {-6, 1},
        {0, 1}, {3, 1}, {4, 1}, {5, 1}, {0, 1}, {0, 1},
        {-6, 1}, {-2, 1}, {-2, 1}, {-1, 1}, {1, 1}, {0, 1},
        {-5, 1}, {-1, 1}, {-2, 1}, {-3, 1}, {0, 1}, {1, 1},
    };
    tab.data = tab_data;

    // Define the basis: it contains the indices of the variables.
    size_t *basis = (size_t*) malloc(tab.m * sizeof(size_t));
    if (basis == NULL) {
        fprintf(stderr, "Error - Not enough memory to allocate the basis\n");
        goto TERMINATE;
    }

    size_t bas[] = {4, 5};
    dual_simplex(&tab, bas);

    //printf("### Starting phase one... ###\n");
    //int status = phase_one(&tab, basis);
    //if (status == FEASIBLE) {
    //    printf("\n### Problem is feasible. Starting phase two... ###\n");
    //    simplex(&tab, basis);
    //}

TERMINATE:
    free_and_null((char**) &basis);

    return 0;
}
