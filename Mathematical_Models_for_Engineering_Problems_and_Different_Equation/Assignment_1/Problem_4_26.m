clear all
format short

A = [[-2, 1, 0];
    [1, -2, 1];
    [0, 1, -1.5]];

B = [[4, -1, 0, 1, 0];
    [-1, 4, -1, 0, 1];
    [0, -1, 4, -1, 0];
    [1, 0, -1, 4, -1];
    [0, 1, 0, -1, 4]];

InfinityNormA = InfinityNorm(A)
InfinityNormB = InfinityNorm(B)