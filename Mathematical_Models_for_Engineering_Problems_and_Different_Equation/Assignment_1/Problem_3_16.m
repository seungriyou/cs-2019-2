clear all
format long

% Solving Problem_3_2
F = @(x) x - 2 * exp(-x);
% fplot(F, [-1, 5]); % Plotting the function in order to determine the first interval.
a = 0; b = 1;   % Determined two points of the first interval by plotting the function. 
                % This values are given from Problem_3_2_(a), too.
                
xSolution = BisectionRoot(F, a, b)