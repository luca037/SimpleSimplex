#ifndef FRACTION_H
#define FRACTION_H

#include <stdlib.h> // Required for EXIT_FAILURE in basic error handling

// Define the Fraction struct
typedef struct {
    int num;
    int den;
} Fraction;

// Function prototypes

// Create/Initialize a fraction
// Returns a Fraction struct initialized with num and den.
// Handles the case where den is 0.
Fraction fraction_create(int num, int den);

// Arithmetic operations
// These functions take two Fraction structs and return a new Fraction struct.
Fraction fraction_add(Fraction f1, Fraction f2);
Fraction fraction_subtract(Fraction f1, Fraction f2);
Fraction fraction_multiply(Fraction f1, Fraction f2);
Fraction fraction_divide(Fraction f1, Fraction f2); // Handles division by zero fraction

// Comparison functions
// These functions return 1 for true, 0 for false.
// Uses cross-multiplication (ad vs bc). Integer overflow is NOT checked.
int fraction_equal(Fraction f1, Fraction f2);
int fraction_not_equal(Fraction f1, Fraction f2);
int fraction_less(Fraction f1, Fraction f2);
int fraction_less_equal(Fraction f1, Fraction f2);
int fraction_greater(Fraction f1, Fraction f2);
int fraction_greater_equal(Fraction f1, Fraction f2);

// Print function
// Prints the fraction to standard output in the format "num/den".
void fraction_print(Fraction f);

#endif // FRACTION_H
