function [outputArg1, outputArg2, a3] = band(low, high, inputSig, sample_rate)
%Fir Bandpass
%
rp = 3; 
rs = 40;
dev = [(10^(rp/20)-1)/(10^(rp/20)+1) 10^(-rs/20)]; 
low_stop = low - 1500;
high_stop = high + 1500;
[N_FIR,fo,mo,w] = firpmord([low_stop low high high_stop], [0 1 0], [0.05, 0.01, 0.05], sample_rate);
fprintf("N_FIR = %d\n", N_FIR);
coeff = firpm(N_FIR,fo,mo,w);

freqz(coeff,1, 200000, sample_rate)

xb = filter(coeff, 1, inputSig);
outputArg1 = xb;
outputArg2 = coeff;
a3 = fo;
end
