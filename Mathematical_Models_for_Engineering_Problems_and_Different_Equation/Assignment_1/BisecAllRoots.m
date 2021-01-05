function R = BisecAllRoots(fun,a,b,TolMax)
% BisecAllRoots solves for all the real roots in a specified domain of a
% nonlinear function f(x)=0 using the bisection method.
% Input variables:
% Fun     A function that calculates f(x) for a given x.
% a, b    The domain.
% TolMax  The maximum tolerance that is used by the bisection method when
% the value of each root is calculated.
% Output variable:
% R       A vector whose elements are the values of the roots.

xNum = 0; % the number of solutions
n = 10;
h = (b - a) / n;
for i = 1:n
    ai = a + (i - 1) * h;
    bi = a + i * h;
    if fun(ai) * fun(bi) < 0
        X = Bisection(fun, ai, bi, TolMax);
        xNum = xNum + 1;
        xSol(xNum) = X;
    end
end

n = 100;
go = 1; % if go == 1, while-loop goes on.
while go == 1
    go = 0;
    h = (b - a) / n;
    for i = 1:n
        ai = a + (i - 1) * h;
        bi = a + i * h;
        if fun(ai) * fun(bi) < 0
            X = Bisection(fun, ai, bi, TolMax);
            same = 0;
            for j = 1:xNum
                if abs(X - xSol(j)) < TolMax % if new xNS = one of the previous roots
                    same = 1;
                    break
                end
            end
            % same == 1, when new xNS = one of the previous roots, therefore this while-loop should be stopped.
            % same == 0, when new xNS \= one of the previous roots,
            % therefore there are one or more new roots.
            if same == 0   % if one or more new roots have been identified
                xNum = xNum + 1;
                xSol(xNum) = X;
                go = 1;  % repeat this while-loop.
            end            
        end
    end
    R = xSol;
    n = n * 10;
end
end

function x = Bisection(Fun,a,b,Tol)
% Bisection solves for a root of a nonlinear equation f(x)=0 using the bisection method.
Fa = Fun(a); Fb = Fun(b);
if Fa * Fb > 0
    disp('Error: The function has the same sign at points a and b.')
else
    while 1
        xNS = (a + b)/2;
        FxNS = Fun(xNS);
        toli = abs(FxNS); % tolerance in f(x)
        if toli < Tol
            x = xNS;
            break
        end
        if Fun(a) * FxNS < 0
            b = xNS;
        else
            a = xNS;
        end
    end
end
end