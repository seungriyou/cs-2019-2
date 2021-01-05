
clear all;

x0 = 2;	% the point of which derivative is to be evaluated

powers = 1:10;

n = length(powers);

% array of h = [1/2, 1/2^2, 1/2^3, ...]
hs = 2.^(-powers);

% f(x)
f = @(x) (2.^x)./x;

% f'(x)
fd1 = @(x) (2.^x).*log(2)./x - (2.^x)./(x.^2);

% f''(x)
fd2 = @(x) (2.^x).*(log(2).^2)./x - 2.*(2.^x).*log(2)./(x.^2) + 2*(2.^x)./(x.^3);

true_val = fd1(x0);	% exact value

data = zeros(6, n);

data(1,:) = abs(true_val - forward_diff_2_points(f, x0, hs));
data(2,:) = abs(true_val - forward_diff_3_points(f, x0, hs));
data(3,:) = abs(true_val - backward_diff_2_points(f, x0, hs));
data(4,:) = abs(true_val - backward_diff_3_points(f, x0, hs));
data(5,:) = abs(true_val - central_diff_2_points(f, x0, hs));
data(6,:) = abs(true_val - central_diff_4_points(f, x0, hs));

vec_cols = {'red', 'blue', 'green', 'black', 'cyan', 'magenta'};
vec_names = {'Two-Points Forward Difference (O(h))',
			'Three-Points Forward Difference (O(h^2))',
			'Two-Points Backward Difference (O(h))',
			'Three-Points Backward Difference (O(h^2))',
			'Two-Points Central Difference (O(h^2))',
			'Four-Points Central Difference (O(h^4))'};
vec_xticks = {};
for i=1:n
	vec_xticks{i,1} = sprintf('2^(-%d)', powers(i));
end

% plot (absolute) true error with respect to h
hsteps = 1:n;
figure(1);
clf;
title('(absolute) true error w.r.t. h');
xlim([0,n+2]);
set(gca(), 'xtick', hsteps);
set(gca(), 'xtickLabel', vec_xticks);
hold on;
for i=1:6 
	hold on;
	plot(hsteps, data(i,:), 'Color', vec_cols{i}, 'Marker', '.');
end
hold on;
legend(vec_names{1}, vec_names{2}, vec_names{3}, vec_names{4}, vec_names{5}, vec_names{6});

% plot the order of truncation error 
hsteps = 1:(n-1);
figure(2);
clf;
title('log of true relative error');
xlim([0,n]);
ylim([0,7]);
set(gca(), 'xtick', hsteps);
set(gca(), 'xtickLabel', {});
for i=1:6
	hold on;
	plot(hsteps, convert_data(data(i,:)), 'Color', vec_cols{i}, 'Marker', '.');
end
hold on;
legend(vec_names{1}, vec_names{2}, vec_names{3}, vec_names{4}, vec_names{5}, vec_names{6});


function [y] = convert_data(x)
	y = abs(log(x(2:end)./x(1:(end-1)))./log(2));
end

% Two-Points Forward Difference Formula
function [d1] = forward_diff_2_points(f, x, h)
	d1 = (f(x+h) - f(x))./h;
end

% Two-Points Backward Difference Formula
function [d1] = backward_diff_2_points(f, x, h)
    d1 = (f(x) - f(x-h))./h;
end

% Two-Points Central Difference Formula
function [d1] = central_diff_2_points(f, x, h)
    d1 = (f(x+h) - f(x-h))./(2*h);
end

% Three-Points Forward Difference Formula
function [d1] = forward_diff_3_points(f, x, h)
    d1 = (-3*f(x) + 4*f(x+h) - f(x+2*h))./(2*h);
end

% Three-Points Backward Difference Formula
function [d1] = backward_diff_3_points(f, x, h)
    d1 = (f(x-2*h) - 4*f(x-h) + 3*f(x))./(2*h);
end

% Four-Points Central Difference Formula
function [d1] = central_diff_4_points(f, x, h)
    d1 = (f(x-2*h) - 8*f(x-h) + 8*f(x+h) - f(x+2*h))./(12*h);
end