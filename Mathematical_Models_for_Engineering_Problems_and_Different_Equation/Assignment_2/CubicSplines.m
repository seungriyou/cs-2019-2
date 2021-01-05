function Yint = CubicSplines(x,y,xint)
% CubicSplines is a function for interpolation with natural cubic splines.
% Input variables:
% x     Vector with the coordinates x of the data points.
% y     Vector with the coordinates y of the data points.
% xint  The x coordinate of the interpolated point.
% Output variables:
% Yint  The y value of the interpolated point.

nx = length(x);
ny = length(y);
if nx ~= ny
    disp('ERROR: The number of elements in x must be the same as in y.')
    Yint = 'Error';
else
    % setting vector h and vector yDiff
    for i = 1:nx-1
        h(i) = x(i+1) - x(i);
        yDiff(i) = y(i+1) - y(i);
    end
    % setting matrix a
    a = zeros(nx-2);
    for i = 1:nx-2
        a(i,i) = 2 * (h(i) + h(i+1));
    end
    for i = 2:nx-2
        a(i-1,i) = h(i);
        a(i,i-1) = h(i);
    end
    % setting vector b
    b = zeros(nx-2,1);
    for i = 1:nx-2
        b(i,1) = 6 * (yDiff(i+1)/h(i+1) - yDiff(i)/h(i));
    end
    % solution vector coef
    coef = (a\b)';
    coef = [0 coef 0];
    % after this step, xint is in the i-th subinterval
    i = 1;
    for i = 1:nx-1
        if (xint >= x(i) && xint <= x(i+1))
            break
        end
    end
    % checking whether xint is in the given range
    if (xint < x(1) || xint > x(nx))
        disp('ERROR: The value of xint is not in the range of x.')
        Yint = 'Error';
    else
        Yint = coef(i)/(6*h(i))*(x(i+1)-xint)^3 + coef(i+1)/(6*h(i))*(xint-x(i))^3 + (y(i)/h(i)-coef(i)*h(i)/6)*(x(i+1)-xint) + (y(i+1)/h(i)-coef(i+1)*h(i)/6)*(xint-x(i)); 
    end
end