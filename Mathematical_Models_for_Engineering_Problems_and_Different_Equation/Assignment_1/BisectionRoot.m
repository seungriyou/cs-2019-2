function Xs = BisectionRoot(Fun,a,b)
% BisectionRoot solves for a root of a nonlinear equation f(x)=0 using the
% bisection method.
% Input variables:
% Fun   A name for the function that calculates f(x) for a given x.
% a, b  Two points that bracket the root.
% Output variable:
% Xs    Solution.

%i = 1; 
Tol = 0.000001;
Fa = Fun(a); Fb = Fun(b);
if Fa * Fb > 0
    disp('Error: The function has the same sign at points a and b.')
else
    %disp('iteration     a          b      (xNS) Solution  f(xNS)   ToleranceInf')
    while 1
        xNS = (a + b)/2;
        FxNS = Fun(xNS);
        toli = abs(FxNS); % tolerance in f(x)
        %fprintf('%3i    %11.6f %11.6f %11.6f  %11.6f %11.6f\n',i, a, b, xNS, FxNS, toli)
        if FxNS == 0
            Xs = xNS;
            fprintf('An exact solution x = %11.6f was found', Xs)
            break
        end
        if toli < Tol
            Xs = xNS;
            break
        end
        if Fun(a) * FxNS < 0
            b = xNS;
        else
            a = xNS;
        end
        %i = i + 1;
    end
end

