function N = OneNorm(A)
% OneNorm calculates the 1-norm of any matrix.
% Input variable:
% A    The matrix.
% Output variable:
% N    The value of the norm.

[R, C] = size(A);
max = 0;
for j = 1:C
    sum_col = 0;
    for i = 1:R
        sum_col = sum_col + abs(A(i, j));
    end
    if sum_col > max
        max = sum_col; % find the maximum sum_col
    end
end
N = max;
end