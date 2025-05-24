#include "../include/fraction.h"
#include <stdio.h>

// FIXME: add a "constructor".
typedef struct {
    size_t n;  // # of columns of the constraint matrix.
    size_t m;  // # of rows of the constraint matrix.
    Fraction *data; // (m+1) x (n+1) matrix.
} Tableau;

enum tableau_status {
    INFEASIBLE, FEASIBLE, OPTIMAL, UNBOUNDED
};

void free_and_null(char **ptr) {
    if (*ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}

void pivot_operations(Tableau *tab, size_t h, size_t t, int minipivot, size_t row) {
    int cols = tab->n + 1;
    Fraction save = tab->data[t * cols + h];

    for (size_t j = 0; j <= tab->n; j++) {
        tab->data[t * cols + j] = fraction_divide(tab->data[t * cols + j], save);
    }

    if (minipivot) {
        if (row != t && !fraction_equal(tab->data[row * cols + h], fraction_create(0, 1))) {
            save = tab->data[row * cols + h];
            for (size_t j = 0; j <= tab->n; j++) {
                Fraction tmp = fraction_multiply(save, tab->data[t * cols + j]);
                tab->data[row * cols + j] = fraction_subtract(tab->data[row * cols + j], tmp);
            }
        }
    } else {
        for (size_t i = 0; i <= tab->m; i++) {
            if (i != t) {
                save = tab->data[i * cols + h];
                for (size_t j = 0; j <= tab->n; j++) {
                    Fraction tmp = fraction_multiply(save, tab->data[t * cols + j]);
                    tab->data[i * cols + j] = fraction_subtract(tab->data[i * cols + j], tmp);
                }
            }
        }
    }

}

// Print the tableau in a nice way :).
void pretty_print_tableau(Tableau *tab) {
    size_t cols = tab->n + 1;

    for (int i = 0; i <= tab->m; i++) {
        printf("[ ");

        for (int j = 0; j <= tab->n; j++) {
            Fraction *elem = &tab->data[i * cols + j];

            // Buffer that holds the string representation of 'elem'.
            char cell_str[50]; // 50 is a safe size for int/int strings

            if (elem->den == 1) {
                snprintf(cell_str, sizeof(cell_str), "%d", elem->num);
            } else {
                snprintf(cell_str, sizeof(cell_str), "%d/%d", elem->num, elem->den);
            }

            // Print the formatted string with a fixed width, right-aligned.
            printf("%6s ", cell_str);

        }
        printf("]\n");
    }
}

// FIXME: implement the bland's rule.
// Return 1 if the problem is unbounded, 0 otherwise.
// If the problem is not unbounded, then 't' contains the pivot row index.
char unbounded_check(Tableau *tab, size_t h, size_t *t) {
    char unbounded = 1;
    int cols = tab->n + 1;  // Numer of cols of the tableau.
    Fraction min = {-1, 1}; // Used to compute the pivot row.
    Fraction *elem = NULL;  // Element of the tableau.

    for (size_t i = 1; i <= tab->m; i++) {
        elem = &tab->data[i * cols + h];

        if (elem->num > 0) {
            unbounded = 0;

            // 'tmp' is the new minumum candidate.
            Fraction tmp = fraction_divide(tab->data[i * cols], *elem);
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
    char optimal = 1;

    for (size_t j = 1; j <= tab->n; j++) {
        if (tab->data[j].num < 0) { // If a reduced cost is negative.
            optimal = 0;
            *h = j; // j enters the basis.
            break;
        }
    }

    return optimal;
}

int simplex(Tableau *tab, size_t *basis) {
    char unbounded = 0; // True if the problem is unbounded.
    char optimal = 0;   // True if found an optimal solution.

    int itr = 0; // Iteration number.

    size_t h = -1; // Index of the variable that enters the basis.
    size_t t = 0; // Pivot row.

    size_t cols = tab->n + 1;

    // Start the simplex algorithm.
    while (!optimal && !unbounded) {
        printf("Current tableau - itr: %d\n", itr);
        pretty_print_tableau(tab);
 
        // Optimality check.
        optimal = optimality_check(tab, &h);

        if (!optimal) {
            printf("x[%lu] enters the basis.\n", h);

            unbounded = unbounded_check(tab, h, &t);
            if (!unbounded) {
                printf("Current pivot element = ");
                fraction_print(tab->data[t * cols + h]);
                printf("\nx[%lu] leaves the basis.\n", basis[t - 1]);
                pivot_operations(tab, h, t, 0, 0);

                basis[t - 1] = h; // Update basis;
                itr += 1;         // Increment iteration.
                printf("\n");
            }
        }
    }

    // Check the result.
    if (optimal) {
        printf("Found an optimal solution.\n");
        Fraction cost = fraction_multiply(tab->data[0], fraction_create(-1, 1));
        printf("Cost = "); fraction_print(cost); printf("\n");
        return OPTIMAL;
    }

    return UNBOUNDED;
}

// Phase 1 of Two phases simplex method.
int phase_one(Tableau *tab, size_t *basis) {
    int status = INFEASIBLE; // Referred to orginal problem (not artificial).
    // Create the tableau associated to the artificial problem.
    Tableau artificial;
    artificial.n = tab->n + tab->m; // Add 'm' artificial variables.
    artificial.m = tab->m;
    
    // Allocate memory for the tableau.
    size_t cols_a = artificial.n + 1; // Tableau cols - artificial.
    size_t sz = cols_a * (artificial.m + 1); // Tableau size.
    artificial.data = (Fraction*) malloc(sz * sizeof(Fraction));

    if (artificial.data == NULL) {
        fprintf(stderr, "Error - No enough memory to create artificial probelm.");
        goto TERMINATE;
    }

    // Copy the of original tableau.
    size_t cols_o = tab->n + 1; // Tableau cols - original.
    for (size_t i = 1; i <= tab->m; i++) {
        for (size_t j = 0; j <= tab->n; j++) {
            artificial.data[i * cols_a + j] = tab->data[i * cols_o + j];
        }
    }

    // Update row0 to reflect artificial problem objective.
    for (size_t j = 0; j <= artificial.n; j++) {
        if (j > tab->n) {
            artificial.data[j] = fraction_create(1, 1);
        } else {
            artificial.data[j] = fraction_create(0, 1);
        }
    }

    // Identity matrix.
    for (size_t i = 1; i <= tab->m; i++) {
        for (size_t j = tab->n+1; j <= artificial.n; j++) {

            if (i == (j - tab->n)) { // Principal diagonal.
                artificial.data[i * cols_a +  j] = fraction_create(1, 1);
            } else { // All other elements.
                artificial.data[i * cols_a +  j] = fraction_create(0, 1);
            }

        }
    }

    // Bring artificial tableau in his canonical form.
    for (size_t i = 1; i <= artificial.m; i++) {
        pivot_operations(&artificial, tab->n + i, i, 1, 0);
    }

    // Define the basis for the artificial problem.
    for (size_t j = 0; j < artificial.m; j++) {
        basis[j] = j + tab->n + 1;
    }

    // Solve the artificial problem. Don't need to check for the status
    // since the artificial problem is never unbounded.
    simplex(&artificial, basis);

    // Check solution status.
    Fraction cost = fraction_multiply(tab->data[0], fraction_create(-1, 1));
    if (!fraction_equal(cost, fraction_create(0, 1))) {
        printf("Original problem is infeasible\n");
        goto TERMINATE;
    }

    // FIXME: need to check degeneracy.
    // Check degeneracy cases.
    for (size_t i = 0; i < tab->m; i++) {
        if (basis[i] > tab->n) { // Found degeneracy.
            printf("Found degeneracy: variable x[%lu].\n", basis[i]);

            // Find first element in row (i+1) of the tableau that is != 0.
            int remove_line = 1;
            for (size_t j = 1; j <= tab->n; j++) {

                Fraction elem = tab->data[(i+1) * cols_o + j];
                if (!fraction_equal(elem, fraction_create(0, 1))) {
                    printf("x[%lu] enters the basis, x[%lu] leaves.\n", j, basis[i]);
                    pivot_operations(tab, j, i+1, 0, 0);
                    basis[i] = j;    // Update basis.
                    remove_line = 0; // No need to remove the line.
                    break;
                }
            }

            if (remove_line) { // FIXME: manage this case.
                fprintf(stderr, "Error - Matrix A is not full rank...\n");
                goto TERMINATE;
            }

        }
    }

    // Original problem admits a feasible solution.
    status = FEASIBLE;

    // Copy solution to original tableau.
    for (size_t i = 1; i <= tab->m; i++) {
        for (size_t j = 0; j <= tab->n; j++) {
            tab->data[i * cols_o + j] = artificial.data[i * cols_a + j];
        }
    }

    // Bring tableau of original problem to his canonical form.
    for (size_t i = 1; i <= tab->m; i++) {
        pivot_operations(tab, basis[i-1], i, 1, 0);
    }

TERMINATE:
    free_and_null((char**) &artificial.data);

    return status;
}

// FIXME: implement blan's rule.
int dual_optimality_check(Tableau *tab, size_t *t) {
    int optimal = 1;
    int cols = tab->n + 1;

    for (size_t i = 1; i <= tab->m; i++) {
        if (tab->data[i * cols].num < 0) { // If a basic variable is negative.
            optimal = 0;
            *t = i; // i leaves the basis.
            break;
        }
    }

    return optimal;
};

char dual_unbounded_check(Tableau *tab, size_t t, size_t *h) {
    char unbounded = 1;
    int cols = tab->n + 1;  // Numer of cols of the tableau.
    Fraction min = {-1, 1}; // Used to compute the pivot row.
    Fraction *elem = NULL;  // Element of the tableau.

    for (size_t j = 1; j <= tab->m; j++) {
        elem = &tab->data[t * cols + j];

        if (elem->num < 0) {
            unbounded = 0;

            // 'tmp' is the new minumum candidate.
            Fraction tmp = fraction_divide(tab->data[j], *elem);
            tmp = fraction_abs(tmp);
            if (min.num == -1 || fraction_less(tmp, min)) {
                min = tmp; // Update the minumum.
                *h = j;    // Update the leaving candidate.
            }
        }
    }
    return unbounded;
}

int dual_simplex(Tableau *tab, size_t *basis) {
    char unbounded = 0; // True if the problem is unbounded.
    char optimal = 0;   // True if found an optimal solution.

    int itr = 0; // Iteration number.

    size_t h = -1; // Index of the variable that enters the basis.
    size_t t = 0; // Pivot row.

    size_t cols = tab->n + 1;

    // Start the simplex algorithm.
    while (!optimal && !unbounded) {
        printf("Current tableau - itr: %d\n", itr);
        pretty_print_tableau(tab);
 
        // Optimality check.
        optimal = dual_optimality_check(tab, &t);

        if (!optimal) {
            printf("x[%lu] leaves the basis.\n", basis[t - 1]);

            unbounded = dual_unbounded_check(tab, t, &h);

            if (!unbounded) {
                printf("Current pivot element = ");
                fraction_print(tab->data[t * cols + h]);
                printf("\nx[%lu] enters the basis.\n", h);
                pivot_operations(tab, h, t, 0, 0);

                basis[t - 1] = h; // Update basis;
                itr += 1;         // Increment iteration.
                printf("\n");
            }
        }
    }

    // Check the result.
    if (optimal) {
        printf("Found an optimal solution.\n");
        Fraction cost = fraction_multiply(tab->data[0], fraction_create(-1, 1));
        printf("Cost = "); fraction_print(cost); printf("\n");
        return OPTIMAL;
    }

    return UNBOUNDED;
}

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
