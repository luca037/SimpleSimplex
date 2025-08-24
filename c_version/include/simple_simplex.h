#ifndef SIMPLE_SIMPLEX_H
#define SIMPLE_SIMPLEX_H

#include <stdio.h>

#include "../include/fraction.h"
#include "../include/utils.h"

// FIXME: add a "constructor".
typedef struct {
    size_t n;  // # of columns of the constraint matrix.
    size_t m;  // # of rows of the constraint matrix.
    Fraction *data; // (m+1) x (n+1) matrix.
} Tableau;

enum tableau_status {
    INFEASIBLE, FEASIBLE, OPTIMAL, UNBOUNDED
};


// Load the tableau specified by the user.
int load_tableau(const char *num_fn, const char *den_fn, int rows, int cols,
        Tableau *tab);

// Find the starting point for the dual simplex.
int search_starting_basis(Tableau *tab, size_t *basis);

void pivot_operations(Tableau *tab, size_t h, size_t t, int minipivot, size_t row);

// Print the tableau in a nice way :).
void pretty_print_tableau(Tableau *tab, size_t *basis);

// FIXME: implement the bland's rule.
// Return 1 if the problem is unbounded, 0 otherwise.
// If the problem is not unbounded, then 't' contains the pivot row index.
char unbounded_check(Tableau *tab, size_t h, size_t *t, size_t *basis);

// Return 1 if the tablau is optimal, 0 otherwise.
// If the tablau is not optimal, then 'h' contains the index of the
// variable that enters the basis.
char optimality_check(Tableau *tab, size_t *h);

int simplex(Tableau *tab, size_t *basis);

// Phase 1 of Two phases simplex method.
int phase_one(Tableau *tab, size_t *basis);

// FIXME: implement blan's rule.
int dual_optimality_check(Tableau *tab, size_t *t, size_t *basis);

char dual_unbounded_check(Tableau *tab, size_t t, size_t *h);

// Dual simplex algorithm.
int dual_simplex(Tableau *tab, size_t *basis);

// Cutting plane algorithm.
int cutting_plane(Tableau *tab, size_t *basis);

#endif
