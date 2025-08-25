import numpy as np
from fractions import Fraction
import os

# File in which the user specify the data.
import problem_data 

# Numerators file name.
NUM_FN = "./data/numerators"
# Denominators file name.
DEN_FN = "./data/denominators"

# Binary executable path.
exec_cmd = "./build/out"

# Save the denominators and numerators in a binary file.
def matrix_to_bin_file(matrix, num_fn, den_fn):

    rows, cols = matrix.shape

    numerators = np.zeros((rows, cols), dtype=np.int32)
    denominators = np.zeros((rows, cols), dtype=np.int32)

    for i in range(rows):
        for j in range(cols):
            frac = matrix[i, j]
            numerators[i, j] = frac.numerator
            denominators[i, j] = frac.denominator

    # Save as binary files.
    numerators.tofile(num_fn)
    denominators.tofile(den_fn)   


# Returns the tableau.
def construct_tableau(A, b, c):
    # Transform in an np.array.
    A = np.array(A)
    b = np.array(b)
    c = np.array(c)

    # Transform all elements in Fraction objects.
    to_fraction = np.vectorize(lambda x: Fraction(x))
    A = to_fraction(A)
    b = to_fraction(b)
    c = to_fraction(c)

    # Create the full tableau.
    c = np.concatenate([[Fraction()], c])
    Tableau = np.hstack([b.reshape(-1, 1), A])
    Tableau = np.vstack([c, Tableau])

    return Tableau


if __name__ == "__main__":

    # Create the tableau.
    Tableau = construct_tableau(problem_data.A, problem_data.b, problem_data.c)

    # Crate binary files of the data.
    matrix_to_bin_file(Tableau, NUM_FN, DEN_FN)

    # Define the remaning paramters.
    rows, cols = Tableau.shape
    mode = problem_data.mode

    # Execute
    to_execute = f"{exec_cmd} {NUM_FN} {DEN_FN} {rows} {cols} {mode}"
    os.system(to_execute)
