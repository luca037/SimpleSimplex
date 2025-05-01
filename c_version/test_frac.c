#include "./fraction.h"
#include <stdio.h> // Required for printf

int main() {
    // Create Fraction objects - they will be simplified
    Fraction f1 = fraction_create(2, 4);   // Represents 1/2
    Fraction f2 = fraction_create(6, 8);   // Represents 3/4
    Fraction f3 = fraction_create(10, -5); // Represents -2/1
    Fraction f4 = fraction_create(-4, -6); // Represents 2/3
    Fraction f5 = fraction_create(0, 100); // Represents 0/1
    Fraction f6 = fraction_create(1, 0);   // Example of creating with a zero denominator (will print error)


    printf("f1 = "); fraction_print(f1); printf("\n"); // Expected: 1/2
    printf("f2 = "); fraction_print(f2); printf("\n"); // Expected: 3/4
    printf("f3 = "); fraction_print(f3); printf("\n"); // Expected: -2/1
    printf("f4 = "); fraction_print(f4); printf("\n"); // Expected: 2/3
    printf("f5 = "); fraction_print(f5); printf("\n"); // Expected: 0/1
    printf("f6 = "); fraction_print(f6); printf("\n"); // Expected: Error and 0/1

    printf("\n--- Arithmetic Operations (Results will be simplified, overflow NOT checked) ---\n");

    // Perform arithmetic operations
    Fraction sum = fraction_add(f1, f2);
    Fraction difference = fraction_subtract(f1, f2);
    Fraction product = fraction_multiply(f1, f2);
    Fraction quotient = fraction_divide(f1, f2);

    printf("f1 + f2 = "); fraction_print(sum); printf("\n");      // Expected: 5/4
    printf("f1 - f2 = "); fraction_print(difference); printf("\n");// Expected: -1/4
    printf("f1 * f2 = "); fraction_print(product); printf("\n");    // Expected: 3/8
    printf("f1 / f2 = "); fraction_print(quotient); printf("\n");   // Expected: 2/3

    // Example with simplification needed
    Fraction f7 = fraction_add(fraction_create(1, 3), fraction_create(1, 6)); // 1/3 + 1/6 = 2/6 + 1/6 = 3/6 = 1/2
    printf("1/3 + 1/6 = "); fraction_print(f7); printf("\n"); // Expected: 1/2

    // Example of division by zero fraction (will print an error)
    Fraction zero_fraction = fraction_create(0, 5);
    Fraction div_by_zero_example = fraction_divide(f1, zero_fraction);
    printf("f1 / (0/5) = "); fraction_print(div_by_zero_example); printf("\n"); // Expected: Error and 0/1

    return 0;
}
