import numpy as np
import math
from fractions import Fraction
from pprint import pprint

# Define the problem.
# Infeasible prob:
#A = np.array(
#    [[0, -1, 0 , 5, -1, -3, 0],
#    [-1, 0, 0, 1, 0, -4, 0],
#    [0, 0, -1, 0, 1, -3, 0],
#    [0, 0, 0, 0, -2, 3, -1]]
#)
#b = np.array([1, 5, 3, 5])
#c = np.array([0, 0, 0, 4, 3, 1, 0])

#A = np.array(
#    [[6, 4, 1, 0],
#    [3, -2, 0, 1]]
#)
#b = np.array([24, 6])
#c = np.array([-1, -1, 0, 0])

A = np.array(
    [[Fraction(2), Fraction(2), Fraction(1), Fraction(0), Fraction(0)],
    [Fraction(1), Fraction(3), Fraction(0), Fraction(1), Fraction(0)],
    [Fraction(2), Fraction(1), Fraction(0), Fraction(0), Fraction(1)]]
)
b = np.array([Fraction(8), Fraction(7), Fraction(5)])
c = np.array([Fraction(-4), Fraction(-5), Fraction(0), Fraction(0), Fraction(0)])

# Simple es
#A = np.array(
#    [[2, 0, 3],
#    [3, 2, -1]]
#)
#b = np.array([1, 5])
#c = np.array([1, -2, 0])


#def print_vertices(vertices):
#    # Print feasible region.
#    x = np.linspace(0, 5, 5)
#    for i, a in enumerate(A[:, 0:2]):
#        if a[1] != 0:
#            y = - a[0] / a[1] * x + b[i] / a[1]
#            plt.plot(x, y)
#        else:
#            plt.plot([b[i] / a[0], b[i] / a[0]], [0, 5])
#
#    # Print vertices.
#    x1 = [0] * len(vertices)
#    x2 = [0] * len(vertices)
#    for i, basis_sol in enumerate(vertices):
#        for x in basis_sol:
#            if x[0] == 1:
#                x1[i] = x[1]
#            elif x[0] == 2:
#                x2[i] = x[1]
#
#    plt.xlim(0, 10)  # x >= 0
#    plt.ylim(0, 20)  # y >= 0
#    plt.plot(x1, x2, marker='o')
#    plt.show()


# Print the tableau matrix.
def pretty_print_tableau(tableau):
    #print('-' * 50)
    #print('\n'.join(['\t'.join([str(cell) for cell in row]) for row in tableau]))
    #print('-' * 50)
    rows = []
    for row in tableau:
        formatted_row = ["{:>6}".format(str(element)) for element in row]
        rows.append("[" + " ".join(formatted_row) + "]")
    print("\n".join(rows))


def dual_simplex(tableau, n, m, beta, x_opt, vertices=None):
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
        print(f"x_B = {[f"x[{i}]" for i in beta]} = {[str(tableau[i, 0]) for i in range(1, m + 1)]}.")
        # Save optimal solution, if necessary.
        if x_opt is not None:
            for i, v in enumerate([tableau[i, 0] for i in range(1, m + 1)]):
                x_opt[i] = v
        minus_bar_c0 = -1 * tableau[0, 0]
        print(f"Optimal value: {minus_bar_c0}")
        return minus_bar_c0

    print("The problem is unbounded.")
    return float('-inf')


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


# Simplex Method. This implementation uses the Bland's rule.
def simplex(tableau, n, m, beta, x_opt=None):
    unbounded = False
    optimal = False

    itr = 0 # Num of iteration.

    while not optimal and not unbounded:
        # Save the current vertex, if necessary.
        #if vertices is not None:
        #    vertex = [[beta[i-1], tableau[i, 0].tolist()] for i in range(1, m + 1)]
        #    #vertex = [tableau[i, 0].tolist() for i in range(1, m + 1)]
        #    print("vertex =", vertex)
        #    vertices.append(vertex)

        print(f"Current tableau - Itr: {itr}")
        pretty_print_tableau(tableau)
        # Optimality test.
        h = -1 # Index of the column that enters the basis.
        optimal = True
        for j in range(1, n + 1):
            if tableau[0, j] < 0:
                optimal = False
                h = j # Choose the var that eneters the basis (Bland's rule).
                print(f"x[{h}] enters the basis.")
                break
        if not optimal:
            # Unbounded check.
            unbounded = True
            for i in range(1, m + 1):
                if tableau[i, h] >= 0:
                    unbounded = False
                    break
            if not unbounded:
                bi_over_ai = np.zeros(m) # Stores the theta candidates.
                for i in range(1, m + 1):
                    if tableau[i, h] > 0:
                        bi_over_ai[i-1] = tableau[i, 0] / tableau[i, h]
                    else:
                        bi_over_ai[i-1] = float('inf') # Otherwise i can't use 
                                                       # the index returned 
                                                       # by argmin in the tableau.
                                                       # (Sort of placeholder)
                
                # Choose the columns that leaves the basis according to Bland's rule.
                candidates, = np.nonzero(bi_over_ai == bi_over_ai.min())
                t = candidates[0]
                for c in candidates[1:]:
                    if beta[c] < beta[t]:
                        t = c
                t += 1 # Need this to access the corresponding row in the tableau.

                print("Current pivot element =", tableau[t, h])

                pivot(tableau, n, m, t, h) # Update the tableau.

                print(f"x[{beta[t-1]}] leaves the basis.")
                beta[t-1] = h # Update the basis.
                itr += 1
                print()

    # Print the optimal solution, if necessary.
    if optimal:
        print("Optimal solution:")
        print(f"x_B = {[f"x[{i}]" for i in beta]} = {[str(tableau[i, 0]) for i in range(1, m + 1)]}.")
        minus_bar_c0 = -1 * tableau[0, 0]
        # Save solution, if necessary.
        if x_opt is not None:
            for i, v in enumerate([tableau[i, 0] for i in range(1, m + 1)]):
                x_opt[i] = v
        print(f"Optimal value: {minus_bar_c0}")
        return minus_bar_c0

    print("The problem is unbounded.")
    return float('-inf')


# The phase1: find a feasible basis.
def phase1(tableau, n, m, beta):
    original_row0 = tableau[0, :] # Save the original row0.

    # Add to the tableau the identity matrix (y_i variables) and the row0.
    id = np.identity(m, dtype=object)
    for i in range(m):
        for j in range(m):
            id[i, j] = Fraction(id[i, j])
    id = np.vstack(([Fraction(1) for _ in range(m)], id))
    tableau = np.concatenate((tableau, id), axis=1)
    for j in range(n + 1):
        tableau[0, j] = Fraction(0)


    # Update the tableau to transform it in his canonical form.
    for i in range(1, m + 1):
        pivot(tableau, n + m, m, i, n + i, minipivot=True)

    # Solve the artificial problem.
    for i in range(m):
        beta.append(n + i + 1)
    cost = simplex(tableau, n + m, m, beta)

    # Check cost value.
    if cost != 0:
        print("The original problem is infeasible")
        return None

    # Check degeneracy cases.
    for i, var in enumerate(beta):
        if var > n:
            print(f"Degeneracy found: variable x[{var}].")
            remove_line = True
            # Find the value of the first bar_aij != 0
            for j in range(1, n + 1):
                if tableau[i+1, j] != 0:
                    pivot(tableau, n + m, m, i+1, j)
                    beta[i] = j
                    print(f"x[{j}] enters and x[{var}] leaves the basis.")
                    remove_line = False # No need to remove the line (A is full rank)
                    break
            if remove_line: # A is not full rank.
                tableau = np.delete(tableau, i+1)


    # Remove the y_i cols and restore the original row0.
    tableau = np.delete(tableau, [n + i + 1 for i in range(m)], axis=1)
    tableau[0, :] = original_row0

    # Update the tableau to transform it in his canonical form.
    for i in range(1, m + 1):
        #print(i, beta[i-1]) # TODO: ERROR: the beta vector must be updated.
        pivot(tableau, n, m, i, beta[i - 1], minipivot=True)

    # Proceed with PHASE 2.
    return tableau


# Check if solution x is integer feasible.
def check_integrality(x, fracidx=None):
    f = True
    for i, v in enumerate(x):
        if not v.is_integer():
            f = False
            if fracidx is not None:
                fracidx.append(i)
    return f


def cutting_plane(A, b, c):
    m, n = A.shape # Number of rows and columns

    # Init the tableau in his original form.
    # Add row0.
    tableau = np.array(np.insert(c, 0, Fraction(0)))
    # Add b and A.
    tableau = np.vstack((tableau, np.column_stack((b, A))))

    # Solve the linear relaxation with the simplex.
    print("### Solve the linear relaxation - PHASE 1 ###")
    beta = [] # For the basic variable.
    tableau = phase1(tableau, n, m, beta)
    if tableau is None:
        return

    print("\n### Solve the linear relaxation - PHASE 2 ###")
    x_opt = [0] * m
    z_opt = simplex(tableau, n, m, beta, x_opt)
    if float("-inf") == z_opt:
        return


    fracidx = [] # Store index of fractional variables. Cutting plane.
    while not check_integrality(x_opt, fracidx): 
        # Find the Gomery's cut, choose the first non integer variable.
        # First compute the values on the new row.
        new_row = np.zeros(tableau.shape[1] + 1, dtype=object)
        new_row[-1] = 1 # Last element is always a one.

        # Selects the first fractional variable.
        for i, v in enumerate(tableau[fracidx[0] + 1, :]):
            new_row[i] = Fraction(- (v - math.floor(v))) # The fractional 
                                                         # part of x 
                                                         # (i.e. phi(x))

        # Add a new column to the tableau.
        tableau = np.column_stack((tableau, [0] * tableau.shape[0]))
        # Add the new row to the tableau.
        tableau = np.vstack((tableau, new_row))
        #print("### AUGMENTED TABLEAU ###")
        #pretty_print_tableau(tableau)

        # Add new element to the basis and to the optimal solution list.
        beta.append(tableau.shape[1] - 1)
        x_opt.append(0)

        # Update number of rows and columns.
        n += 1
        m += 1

        # Solve the linear relaxation (dual simplex).
        print("\n### Dual simplex ###")
        dual_simplex(tableau, n, m, beta, x_opt)
        
        # Clear the fractinal index list.
        fracidx = []

    
def main():
    cutting_plane(A, b, c)


if __name__ == "__main__":
    main()
