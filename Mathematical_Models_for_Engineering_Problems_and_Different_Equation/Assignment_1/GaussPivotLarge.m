function x = GaussPivotLarge(a,b)
% In each step of the elimination the pivot row is switched with the row
% that has a pivot element with the largest absolute numerical value.
% Input variables:
% a   The matrix of coefficients.
% b   The right-hand-side column of constants.
% Output variable:
% x   The solution.

ab = [a, b];
[R, C] = size(ab);
for j = 1:R-1
    % Pivoting section starts
    for k = j+1:R
        if abs(ab(k, j)) > abs(ab(j, j)) % compare to absolute numerical value of the pivot element of the current row
            abTemp = ab(j, :);
            ab(j, :) = ab(k, :);
            ab(k, :) = abTemp;
            % There should be no break here, because we have to compare all
            % rows below the current row.
        end
    end
    % Pivoting section ends
    for i = j+1:R
        ab(i, j:C) = ab(i, j:C) - ab(i, j) / ab(j, j) * ab(j, j:C);
    end
end
x= zeros(R, 1);
x(R) = ab(R, C) / ab(R, R);
for i = R-1:-1:1
    x(i) = (ab(i, C) - ab(i, i+1:R) * x(i+1:R)) / ab(i, i);
end
end