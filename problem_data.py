# Specify the data related to the problem.
# For example, consider the problem:
#
#    min -4x1 - 5x2 
#
#         2x1 + 2x2 + x3           = 8
#          x1 + 3x2      + x4      = 7
#         2x1 +  x2           + x5 = 5
#
#          x1,   x2, x3,   x4,  x5 >= 0 integer

# A matrix.
A = [
        [2, 2, 1, 0, 0],
        [1, 3, 0, 1, 0],
        [2, 1, 0, 0, 1]
]

# b vector.
b = [8, 7, 5]

# c vector.
c = [-4, -5, 0, 0, 0]

# Specify the mode.
# Possible values:
#    - S   => (Primal) Simplex
#    - TPS => Two Phase Simplex
#    - DS  => Dual Simplex
#    - CP  => Cutting Plane
mode = "CP"
