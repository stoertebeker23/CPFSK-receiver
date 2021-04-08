clc
close all
fT_bp = (11100800 + 10100800)/2;%10100800; % Hz
fT = 4790;

B_bp = 11100800 - 10100800;%
B = 704; %Hz
nmax_bp = floor( (2 * fT_bp - B_bp) / 2 / B_bp);
nmax = floor( (2 * fT - B) / 2 / B);

n_bp = (1:1:nmax_bp);
n = (1:1:nmax);

lower_border = (2*fT - B) ./ n;
upper_border = (2*fT + B) ./ (n + 1);

lower_border_bp = (2*fT_bp - B_bp) ./ n_bp;
upper_border_bp = (2*fT_bp + B_bp) ./ (n_bp + 1);

ok = 4*fT ./ (2*n+1);
ok_bp = 4*fT_bp ./ (2*n_bp+1);

figure(1)
plot(n, lower_border, n, upper_border, n, ok, 'or')
title("TP")
figure(2)

plot(n_bp, lower_border_bp, n_bp, upper_border_bp, n_bp, ok_bp, 'or')
title("bp")