function [a,Er] = CubicPolyFit(x,y)
% CubicPolyFit determines the coefficients of a cubic polynomial, f(x) =
% a3*x^3 + a2*x^2 + a1*x + a0, that best fits a given set of data points.
% Also it calculates the overall error Er.
% Input variables:
% x    Vector with the coordinates x of the data points.
% y    Vector with the coordinates y of the data points.
% Output variables:
% a    A four-element vector with the values of the coefficients a3, a2,
%      a1, and a0.
% Er   The value of the overall error.

nx = length(x);
ny = length(y);
m = 3;
if nx ~= ny
    disp('ERROR: The number of elements in x must be the same as in y.')
    a = 'Error';
    Er = 'Error';
else  
    for i = 1:2*m
        xsum(i) = sum(x.^(i));
    end
    c(1,1) = nx;
    d(1,1) = sum(y);
    for j = 2:m+1
        c(1,j) = xsum(j-1);
    end
    for i = 2:m+1
        for j = 1:m+1
            c(i,j) = xsum(j+i-2);
        end
        d(i,1) = sum(x.^(i-1).*y);
    end
    coef = (c\d)';
    for i = 1:m+1
        a(i) = coef(3+2-i);
    end
    Er = sum((y - (a(1)*(x.^3) + a(2)*(x.^2) + a(3)*x + a(4))).^2);
end