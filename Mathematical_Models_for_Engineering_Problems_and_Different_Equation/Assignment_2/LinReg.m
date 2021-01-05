function [a,Er] = LinReg(x,y)
% LinReg calculates the coefficients a1 and a0 of the linear equation y =
% a1*x + a0 that best fits n data points. Also it calculates the overall
% error Er.
% Input variables:
% x    Vector with the coordinates x of the data points.
% y    Vector with the coordinates y of the data points.
% Output variables:
% a    A two-element vector with the values of the constants a1 and a0.
% Er   The value of the overall error.

nx = length(x);
ny = length(y);
if nx ~= ny
    disp('ERROR: The number of elements in x must be the same as in y.')
    a = 'Error';
    Er = 'Error';
else
    Sx = sum(x);
    Sy = sum(y);
    Sxy = sum(x.*y);
    Sxx = sum(x.^2);
    a1 = (nx*Sxy - Sx*Sy) / (nx*Sxx - Sx^2);
    a0 = (Sxx*Sy - Sxy*Sx) / (nx*Sxx - Sx^2);
    a = [a1 a0];
    Er = sum((y - (a1*x + a0)).^2);
end