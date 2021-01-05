function Xs = SquareRoot(p)
% Input variable:
% p    The number whose square root is determined.
% Output variable:
% Xs   Answer.

function y = Fun(x)
 y = x^2-p;
end
function y = Fun_Der(x)
y = 2*x;
end
imax = 20; Xest = p;
Err = 1e-6;
if ~(p > 0)
    fprintf('\nError: The number should be a positive number.')
    Xs = ('No answer');
else
    for i = 1:imax
        Xi = Xest - Fun(Xest)/Fun_Der(Xest);
        if abs((Xi - Xest)/Xest) < Err
            Xs = Xi;
            break
        end
        Xest = Xi;
    end
    if i == imax
        fprintf('\nSolution was not obtained in %i iterations.\n', imax)
        Xs = ('No answer');
    end
end
end