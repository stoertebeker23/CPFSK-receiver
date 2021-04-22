function [outputArg1,outputArg2] = custom_hilbert(inputArg1,inputArg2)
%CUSTOM_HILBERT Summary of this function goes here
%   Detailed explanation goes here


% ahgghggg use firpm ....

n = 1:1:1000
sig = sin(2*pi*n*200*0.001)
figure(1)
fft(sig, 1000)
 
hilbert_n_coefficients = 60;
n_hilbert = (-hilbert_n_coefficients/2:1:hilbert_n_coefficients/2);
%fir_hilbert = zeros(hilbert_n_coefficients+1);
fir_hilbert = zeros(1,hilbert_n_coefficients+1,'uint32');
size(fir_hilbert)
size(n_hilbert)
n_hilbert(2:2:hilbert_n_coefficients)
lol = 2/pi./n_hilbert(2:2:hilbert_n_coefficients);

B = reshape([lol; zeros(size(lol))],[],1);
B = [B ;0];

del = [zeros(1, hilbert_n_coefficients/2) sig]
size(fir_hilbert)
figure(10)
size(B)
plot(n_hilbert, B, "-o");

figure(9)
[h,w] = freqz(B,[1],1000);
plot(w/pi, db(abs(h)))
outputArg1 = inputArg1;
outputArg2 = inputArg2;

figure(10)

end

