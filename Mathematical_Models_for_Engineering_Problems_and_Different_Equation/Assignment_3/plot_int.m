
clear all;
close all;

%f = @(x) 97000.*x./(5.*(x.^2) + 570000);
%a = 40;
%b = 93;
%true_val = 574.1494;	% exact value (Example 6-1)

f = @(x) exp(-0.1.*x).*sin(3.*x) + 1;
a = 0;
b = 20;
true_val = 20.3763385576651;

figure(3);
clf;
title('f(x)');
xx = 0:0.01:20;
hold on;
plot(xx, f(xx));

powers = 0:15;

n = length(powers);

% array of h = [1/2, 1/2^2, 1/2^3, ...]
Ns = 6.*(2.^(powers));

err = zeros(5, n);

err(1,:) = abs(true_val - composite_rectangle(f, a, b, Ns));
err(2,:) = abs(true_val - composite_midpoint(f, a, b, Ns));
err(3,:) = abs(true_val - composite_trapezoidal(f, a, b, Ns));
err(4,:) = abs(true_val - composite_Simpson_1_3rd(f, a, b, Ns));
err(5,:) = abs(true_val - composite_Simpson_3_8th(f, a, b, Ns));

vec_cols = {'red', 'blue', 'green', 'black', 'cyan'};
vec_names = {'Composite Rectangle (O(h))',
			'Composite Midpoint (O(h^2))',
			'Composite Trapezoidal (O(h^2))',
			'Composite Simpson''s 1/3 (O(h^4))',
			'Composite Simpson''s 3/8 (O(h^4))'};
vec_xticks = {};
for i=1:n
	vec_xticks{i,1} = sprintf('6*2^%d', powers(i));
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
for i=1:5
	hold on;
	plot(hsteps, err(i,:), 'Color', vec_cols{i}, 'Marker', '.');
end
hold on;
legend(vec_names{1}, vec_names{2}, vec_names{3}, vec_names{4}, vec_names{5});


% plot the order of truncation error 
hsteps = 1:(n-1);
figure(2);
clf;
title('log of true relative error');
xlim([0,n]);
ylim([0,7]);
set(gca(), 'xtick', hsteps);
set(gca(), 'xtickLabel', {});
for i=1:5
	hold on;
	plot(hsteps, convert_data(err(i,:)), 'Color', vec_cols{i}, 'Marker', '.');
end
hold on;
legend(vec_names{1}, vec_names{2}, vec_names{3}, vec_names{4}, vec_names{5});

	
function [y] = convert_data(x)
	y = abs(log(x(2:end)./x(1:(end-1)))./log(2));
end

% Composite Rectangle Method
function [I] = composite_rectangle(f, a, b, Ns)
	for i=1:length(Ns)
		N = Ns(i);
		h = (b-a)/N;
		w = h*ones(1,N+1);
		w(N+1) = 0;
		x = linspace(a, b, N+1);	% x = [x_1, x_2, x_3, ..., x_{N+1} ]
			% x1 = a, x2 = a+h, x3 = a+2h, ...
		I(i) = w*f(x)';	% dot product of w and f(x) = sum_{i=1}^{N+1} w(i)f(x_i)
			% f(x) = [f(x_1), f(x_2), ..., f(x_{N+1})]
	end
end

% Composite Midpoint Method
function [I] = composite_midpoint(f, a, b, Ns)
    for i=1:length(Ns)
        N = Ns(i);
        h = (b-a)/N;
        w = h*ones(1,N);
        x = linspace(a, b, N+1);
        midval = zeros(N,1);
        for j=1:N
            midval(j) = f((x(j)+x(j+1))/2);
        end
        I(i) = w*midval;
    end   
end

% Composite Trapezoidal Method
function [I] = composite_trapezoidal(f, a, b, Ns)
    for i=1:length(Ns)
		N = Ns(i);
		h = (b-a)/N;
		w = h*ones(1,N+1);
		w(1) = h/2;
		w(N+1) = h/2;
		x = linspace(a, b, N+1);
		I(i) = w*f(x)';
	end
end

% Composite Simpson's 1/3 Method
function [I] = composite_Simpson_1_3rd(f, a, b, Ns)
	for i=1:length(Ns)
		N = Ns(i);
		h = (b-a)/N;
		w = zeros(1,N+1);
		w(2:2:N) = 4*h/3;
		w(3:2:N) = 2*h/3;
		w(1) = h/3;
		w(N+1) = h/3;
		x = linspace(a, b, N+1);
		I(i) = w*f(x)';
	end
end

% Composite Simpson's 3/8 Method
function [I] = composite_Simpson_3_8th(f, a, b, Ns)
    for i=1:length(Ns)
		N = Ns(i);
		h = (b-a)/N;
		w = zeros(1,N+1);
        w(2:3:N) = 9*h/8; 
        w(3:3:N) = 9*h/8;
        w(4:3:N) = 6*h/8;
		w(1) = 3*h/8;
		w(N+1) = 3*h/8;
		x = linspace(a, b, N+1);
		I(i) = w*f(x)';
	end
end