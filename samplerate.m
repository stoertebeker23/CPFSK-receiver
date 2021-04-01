clc
close all
fT = 10100800; % Hz
B = 704; %Hz
nmax = floor( (2 * fT - B) / 2 / B);


n = (100:100:nmax);

lower_border = (2*fT - B) ./ n;
upper_border = (2*fT + B) ./ (n + 1);
ok = 4*fT ./ (2*n+1);

plot(n, lower_border, n, upper_border, n, ok, 'or')
