import numpy as np
import matplotlib.pyplot as plt

# Define the problem.
A = np.array(
    [[-2., -2., -1., 1., 0.],
     [-1., -2., -3., 0., 1.]]
)
b = np.array([-6., -5.])
c = np.array([3., 4., 5., 0., 0.])


# Print the tableau matrix.
def pretty_print_tableau(tableau):
    print('-' * 50)
    print('\n'.join(['\t'.join([str(cell) for cell in row]) for row in tableau]))
    print('-' * 50)


# Pivot operations.
def pivot(tableau, n, m, t, h, minipivot=False):
    save = tableau[t, h]
    for j in range(n + 1):
        tableau[t, j] = tableau[t, j] / save
    for i in range(m + 1):
        if i != t and tableau[i, h] != 0:
            save = tableau[i, h]
            for j in range(n + 1):
                tableau[i, j] = tableau[i, j] - save * tableau[t, j]
                # Floating point are bastards.
                #if abs(tableau[i, j]) < 0.000000001:
                #    tableau[i, j] = 0
        if minipivot:
            break


# Dual Simplex Method. This implementation uses the Bland's rule.
def dual_simplex(tableau, n, m, beta):
    unbounded = False
    optimal = False

    itr = 0 # Num of iteration.

    while not optimal and not unbounded:
        print(f"Current tableau - Itr: {itr}")
        pretty_print_tableau(tableau)

        candidates = [] # Save all the basic variables < 0.
        # Optimality check.
        optimal = True
        for i in range(1, m + 1):
            if tableau[i, 0] < 0:
                optimal = False
                candidates.append(i)

        if not optimal:
            # Choose the var that leaves that basis, according to Bland's rule
            candidates = np.array(candidates)
            t = candidates[0]
            for c in candidates[1:]:
                if beta[c-1] < beta[t]:
                    t = c
            print(f"x[{beta[t-1]}] leaves the basis.")

            # Unbounded check.
            unbounded = True
            for j in range(1, n + 1):
                if tableau[t, j] < 0:
                    unbounded = False
                    break
            if not unbounded:
                cj_over_aj = np.zeros(n) # Stores the candidates values.
                for j in range(1, n + 1):
                    if tableau[t, j] < 0:
                        cj_over_aj[j-1] = tableau[0, j] / abs(tableau[t, j])
                    else:
                        cj_over_aj[j-1] = float('inf') # Otherwise i can't use 
                                                       # the index returned 
                                                       # by argmin in the tableau.
                                                       # (Sort of placeholder)
                
                # Choose the columns that enters the basis according to Bland's rule.
                h = np.argmin(cj_over_aj)
                h += 1 # Need this to access the corresponding cols in the tableau.

                print("Current pivot element =", tableau[t, h])

                pivot(tableau, n, m, t, h) # Update the tableau.

                print(f"x[{h}] enters the basis.")
                beta[t-1] = h # Update the basis.
                itr += 1
                print()

    # Print the optimal solution, if necessary.
    if optimal:
        print("Optimal solution:")
        print(f"x_B = {[f"x[{i}]" for i in beta]} = {[tableau[i, 0].tolist() for i in range(1, m + 1)]}.")
        minus_bar_c0 = -1 * tableau[0, 0]
        print(f"Optimal value: {minus_bar_c0}")
        return minus_bar_c0

    print("The problem is unbounded.")
    return float('-inf')


def main():
    # Specify the basis and the non-basic variables.
    beta = [4, 5]

    # Number of rows and columns
    m, n = A.shape 

    # Init the tableau in his original form.
    # Add row0.
    tableau = np.array(np.insert(c, 0, 0))
    # Add b and A.
    tableau = np.vstack((tableau, np.column_stack((b, A))))

    dual_simplex(tableau, n, m, beta)


if __name__ == "__main__":
    main()
