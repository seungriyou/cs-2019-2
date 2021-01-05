function c = CondNumb_One(A)
% CondNumb_One calculates the condition number of an (n x n) matrix by
% using the 1-norm.
% Input variable:
% A    The matrix.
% Output variable:
% c    The value of the condition number.

OneNormA = OneNorm(A);
OneNormAinv = OneNorm(inv(A));
c = OneNormA * OneNormAinv;
end