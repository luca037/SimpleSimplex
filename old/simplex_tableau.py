import numpy as np

# Define the problem.
#A = np.array(
#    [[6, 4, 1, 0],
#    [3, -2, 0, 1]]
#)
#b = np.array([24, 6])
#c = np.array([-1, -1, 0, 0])

A = np.array(
    [[2, 0, 3],
    [3, 2, -1]]
)
b = np.array([1, 5])
c = np.array([1, -2, 0])


def pivot(tableau, n, m, t, h):
    save = tableau[t, h]
    for j in range(n + 1):
        tableau[t, j] = tableau[t, j] / save
    for i in range(m + 1):
        if i != t and tableau[i, h] != 0:
            save = tableau[i, h]
            for j in range(n + 1):
                tableau[i, j] = tableau[i, j] - save * tableau[t, j]


def simplex_tableau(beta, f):
    # Init the tableau in his canonical form.
    print("Init the tableau in his canonical form.")
    inv_B = np.linalg.inv(A[:, beta])
    #print("inv_B:\n", inv_B)

    bar_b = np.dot(inv_B, b)
    bar_F = np.dot(inv_B, A[:, f])
    print("\nbar_b:\n", bar_b)
    #print("bar_F:\n", bar_F)

    minus_bar_c0 = np.dot(np.dot(c[beta], inv_B), b)
    #print("minus_bar_c0 = ", minus_bar_c0)

    bar_cF = c[f] - np.dot(np.dot(c[beta], inv_B), A[:, f])
    #print("bar_cF:\n", bar_cF)
    
    # create the first row
    row0 = np.insert(np.array([0] * len(beta)), 0, -1 * minus_bar_c0)
    row0 = np.concatenate((row0, bar_cF))
    tableau = np.array([row0])

    # create the remaning m rows
    m = len(beta) # Number of rows.
    for i in range(m):
        tmp = [0.] * m
        tmp[i] = 1.
        rowi = np.insert(tmp, 0, bar_b[i])
        rowi = np.concatenate((rowi, bar_F[i, :]))
        tableau = np.vstack([tableau, rowi])
    print("\nInitial tableau:\n", tableau)
    print()

    # start the Simplex algorithm
    col_indices = np.concatenate((beta, f)) # The labels of the columns.
    n = len(A[0]) # Number of columns.

    unbounded = False
    optimal = False

    itr = 0 # Total iteration.

    while not optimal and not unbounded:
        # Optimality test.
        h = -1 # Index of the column that enters the basis.
        optimal = True
        for j in range(1, n + 1):
            if tableau[0, j] < 0:
                optimal = False
                h = j # Choose the var that eneters the basis.
                break
        if not optimal:
            # Unbounded check.
            unbounded = True
            for i in range(1, m + 1):
                if tableau[i, h] >= 0:
                    unbounded = False
                    break
            if not unbounded:
                bi_over_ai = [] # Stores the theta candidates.
                for i in range(1, m + 1):
                    if tableau[i, h] > 0:
                        bi_over_ai.append(tableau[i, 0] / tableau[i, h])
                    else:
                        bi_over_ai.append(float('inf')) # Otherwise i can't use 
                                                        # the index returned 
                                                        # by argmin in the tableau.
                t = np.argmin(bi_over_ai) + 1 # Column that leaves the basis.

                print("Current pivot element =", tableau[t, h])

                pivot(tableau, n, m, t, h) # Update the tableau

                print(f"x[{col_indices[h-1]}] enters the basis.")
                print(f"x[{beta[t-1]}] leaves the basis.")
                beta[t-1] = col_indices[h-1] # Update the basis.
                print(f"\nTableau updated - Itr: {itr}\n", tableau)
                itr += 1

    # Print the optimal solution, if necessary.
    if optimal:
        print("\nOptimal solution:")
        print(f"x_B = {[f"x[{i}]" for i in beta]} = {[tableau[i, 0].tolist() for i in range(1, m + 1)]}.")
        print(f"Optimal value: {-1 * tableau[0, 0]}")
    else:
        print("The problem is unbounded.")


def main():
    # Specify the basis and the non-basic variables.
    beta = np.array([0, 1])
    f = np.array([2])
    simplex_tableau(beta, f)


if __name__ == "__main__":
    main()
