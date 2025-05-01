#include "fraction.h"
#include <stdio.h>

typedef struct {
    size_t n;  // # of columns of the constraint matrix.
    size_t m;  // # of rows of the constraint matrix.
    Fraction *data; // (m+1) x (n+1) matrix.
} Tableau;

void pretty_print_tableau(Tableau *tab) {
    Fraction *elem = NULL; // Pointer to an element of the tablau.
    size_t cols = tab->n + 1;

    for (int i = 0; i <= tab->m; i++) {
        printf("[ ");
        for (int j = 0; j <= tab->n; j++) {
            // Print a space before the element if it's not the first one in the row.
            if (j > 0) {
                printf(" ");
            }
            // Print the element formatted to occupy 6 characters, right-aligned.
            elem = &tab->data[i * cols + j]; // Get the current element.
            printf("%6d/%d", elem->num, elem->den);
        }
        printf(" ]\n"); // Closing bracket and newline
    }
}

// Return 1 if the problem is unbounded, 0 otherwise.
// If the problem is not unbounded, then 't' contains the pivot row index.
char unbounded_check(Tableau *tab, size_t h, size_t *t) {
    char unbounded = 1;
    int cols = tab->n + 1;  // Numer of cols of the tableau.
    Fraction min = {-1, 1}; // Used to compute the pivot row.
    Fraction tmp = {0 , 1};
    Fraction *elem = NULL;  // Element of the tableau.

    for (size_t i = 1; i <= tab->m; i++) {
        elem = &tab->data[i * cols + h];

        if (elem->num > 0) {
            unbounded = 0;

            // 'tmp' is the new minumum candidate.
            tmp = fraction_divide(tab->data[i * cols], *elem);
            if (min.num == -1 || fraction_less(tmp, min)) {
                min = tmp; // Update the minumum.
                *t = i;    // Update the pivot row.
            }
        }
    }
    return unbounded;
}

// Return 1 if the tablau is optimal, 0 otherwise.
// If the tablau is not optimal, then 'h' contains the index of the
// variable that enters the basis.
char optimality_check(Tableau *tab, size_t *h) {
    char optimal = 0;

    for (size_t j = 1; j <= tab->n; j++) {
        if (tab->data[j].num < 0) { // If a reduced cost is negative.
            optimal = 0;
            *h = j; // j enters the basis.
            break;
        }
    }

    return optimal;
}

int simplex(Tableau *tab, int *basis) {
    char unbounded = 0; // True if the problem is unbounded.
    char optimal = 0;   // True if found an optimal solution.

    int itr = 0; // Iteration number.

    size_t h = -1; // Index of the variable that enters the basis.
    size_t t = 0; // Pivot row.

    size_t cols = tab->n + 1;

    while (!optimal && !unbounded) {
        printf("Current tableau - Itr: %d", itr);
        pretty_print_tableau(tab);
 
        // Optimality check.
        optimal = optimality_check(tab, &h);

        if (!optimal) {
            printf("x[%lu] enters the basis.\n", h);

            unbounded = unbounded_check(tab, h, &t);
            if (!unbounded) {
                printf("Current pivot element = %d/%d\n", tab->data[t * cols + h].num, tab->data[t * cols + h].den); // FIXME: improve printing.
                printf("x[%d] leaves the basis.\n", basis[t - 1]);
            }
        }
        break;
    }

    return 0;
}

int main(void) {
    // Consider a simple problem:
    //      min -x1 - x2
    //          6x1 + 4x2 + x3 = 24
    //          3x1 - 2x2 + x4 = 6
    //          x1, x2, x3, x4 >= 0
    
    // Define the tableau.
    Tableau tab;
    tab.n = 4;
    tab.m = 2;

    //const size_t sz = (tab.n + 1) * (tab.m + 1);

    // Define the entries of the tableau (p.47).
    Fraction tab_data[] = {
        {0, 1}, {-1, 1}, {-1, 1}, {0, 1}, {0, 1},
        {24, 1}, {6, 1}, {4, 1}, {1, 1}, {0, 1},
        {6, 1}, {3, 1}, {-2, 1}, {0, 1}, {1, 1},
    };
    tab.data = tab_data;

    // Define the basis: it contains the indices of the variables.
    int basis[] = {3, 4};

    //pretty_print_tableau(&tab);

    simplex(&tab, basis);

    return 0;
}
