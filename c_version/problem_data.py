# Specify the data related to the problem.
# For example, consider the problem:
#
#    min  x1 + x2 + 2x3 + 4x4
#             2x2       - 3x4  = 1
#         x1            -  x4  = 0
#        -x1      +  x3        = 1

# A matrix.
A = [
        [0, 2, 0, -3],
        [1, 0, 0, -1],
        [-1, 0, 1, 0]
]

# b vector.
b = [1, 0, 1]

# c vector.
c = [1, 1, 2, 4]

# Specify the mode.
# Possible values:
#    - TPS => Two Phase Simplex
#    - DS  => Dual Simplex
mode = "TPS"

# NOTE: if you want to use the Dual Simplex (DS), then you must provide 
# the starting basis. In the basis vector you need to specify the index 
# of the variables.
basis = [1, 2]
