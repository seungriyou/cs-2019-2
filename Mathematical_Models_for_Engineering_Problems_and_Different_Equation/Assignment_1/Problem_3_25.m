clear all
format long

F = @(x) x^4 - 5.5*x^3 - 7.2*x^2 + 43*x + 36;
% fplot(F, [-3, 5]);    % Plotting the function in order to determine the first interval.
xRoots = BisecAllRoots(F, -3, 5, 0.0001)    % Determined two points of the first interval by plotting the function.