#include "../include/fraction.h"
#include <stdio.h>  // For fprintf, printf
#include <stdlib.h> // For abs

// Helper function to calculate the Greatest Common Divisor (GCD)
// Uses the Euclidean algorithm. Handles negative numbers by using abs().
int gcd(int a, int b) {
    // Use absolute values for GCD calculation
    a = abs(a);
    b = abs(b);

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to create/initialize and simplify a fraction
Fraction fraction_create(int num, int den) {
    Fraction f;
    f.num = num;
    f.den = den;

    // Basic check for division by zero when creating
    if (f.den == 0) {
        fprintf(stderr, "Error (fraction_create): Denominator cannot be zero. Setting to 0/1.\n");
        f.num = 0;
        f.den = 1; // Represent as 0/1 in case of error
        return f; // Return immediately after handling error
    }

    // Handle the sign: Ensure the.den is positive
    if (f.den < 0) {
        f.num = -f.num;
        f.den = -f.den;
    }

    // Simplify the fraction using GCD
    if (f.num != 0) { // No need to simplify if.num is 0
        int common_divisor = gcd(f.num, f.den);
        f.num /= common_divisor;
        f.den /= common_divisor;
    } else {
        // If.num is 0, simplify to 0/1
        f.den = 1;
    }

    return f;
}

// Addition: (a/b) + (c/d) = (ad + bc) / bd
// Uses only int. Integer overflow is NOT checked.
// The result is created and simplified by fraction_create.
Fraction fraction_add(Fraction f1, Fraction f2) {
    int result_num = (f1.num * f2.den) + (f2.num * f1.den);
    int result_den = f1.den * f2.den;
    // Note: Intermediate multiplication might overflow if inputs are large.
    return fraction_create(result_num, result_den);
}

// Subtraction: (a/b) - (c/d) = (ad - bc) / bd
// Uses only int. Integer overflow is NOT checked.
// The result is created and simplified by fraction_create.
Fraction fraction_subtract(Fraction f1, Fraction f2) {
    int result_num = (f1.num * f2.den) - (f2.num * f1.den);
    int result_den = f1.den * f2.den;
    // Note: Intermediate multiplication/subtraction might overflow if inputs are large.
    return fraction_create(result_num, result_den);
}

// Multiplication: (a/b) * (c/d) = ac / bd
// Uses only int. Integer overflow is NOT checked.
// The result is created and simplified by fraction_create.
Fraction fraction_multiply(Fraction f1, Fraction f2) {
    int result_num = f1.num * f2.num;
    int result_den = f1.den * f2.den;
    // Note: Intermediate multiplication might overflow if inputs are large.
    return fraction_create(result_num, result_den);
}

// Division: (a/b) / (c/d) = ad / bc
// Handles division by zero fraction (c/d where c is 0).
// Uses only int. Integer overflow is NOT checked.
// The result is created and simplified by fraction_create.
Fraction fraction_divide(Fraction f1, Fraction f2) {
    // Basic check for division by zero fraction (c/d where c is 0)
    if (f2.num == 0) {
        fprintf(stderr, "Error (fraction_divide): Division by zero fraction.\n");
        // Return a default fraction (e.g., 0/1) to indicate error
        return fraction_create(0, 1);
    }

    int result_num = f1.num * f2.den;
    int result_den = f1.den * f2.num;
    // Note: Intermediate multiplication might overflow if inputs are large.

    return fraction_create(result_num, result_den);
}

// Absolute value: |(a/b)| = |a| / |b|.
// The result is created and simplified by fraction_create.
Fraction fraction_abs(Fraction f) {
    int num = f.num, den = f.den;
    if (num < 0) {
        num *= -1;
    } else if (den < 0) {
        den *= -1;
    }

    return fraction_create(num, den);
}

// Change sign: -(a/b) = -1/1 * (a/b)
// The result is created and simplified by fraction_create.
Fraction fraction_chg_sign(Fraction f) {
    return fraction_multiply(f, fraction_create(-1, 1));
}

// Comparison functions (New implementations)
// Use cross-multiplication: compare f1.num * f2.den vs f2.num * f1.den
// Assumes denominators are positive due to fraction_create.
// Integer overflow in intermediate products is NOT checked.
int fraction_equal(Fraction f1, Fraction f2) {
    return (f1.num * f2.den) == (f2.num * f1.den);
}

int fraction_not_equal(Fraction f1, Fraction f2) {
    return !fraction_equal(f1, f2);
}

int fraction_less(Fraction f1, Fraction f2) {
    return (f1.num * f2.den) < (f2.num * f1.den);
}

int fraction_less_equal(Fraction f1, Fraction f2) {
    return (f1.num * f2.den) <= (f2.num * f1.den);
}

int fraction_greater(Fraction f1, Fraction f2) {
    return (f1.num * f2.den) > (f2.num * f1.den);
}

int fraction_greater_equal(Fraction f1, Fraction f2) {
    return (f1.num * f2.den) >= (f2.num * f1.den);
}


// Function to print a fraction
void fraction_print(Fraction f) {
    printf("%d/%d", f.num, f.den);
}
