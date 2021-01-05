function N = InfinityNorm(A)
% InfinityNorm calculates the infinity norm of any matrix.
% Input variable:
% A    The matrix.
% Output variable:
% N    The value of the norm.

[R, C] = size(A);
max = 0;
for i = 1:R
    sum_row = 0;
    for j = 1:C
        sum_row = sum_row + abs(A(i, j));
    end
    if sum_row > max  % find the maximum sum_row
        max = sum_row;
    end
end
N = max;
end