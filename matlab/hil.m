function [outputArg1] = hil(inputSig)
%HIL Summary of this function goes here
%   Detailed explanation goes here

coeff = firpm(22,[0.1 0.9],[1 1],'h');  % Bandpass Hilbert
xh = filter(coeff,1, inputSig); % hilber transform of inputSig

xd = [zeros(11,1); inputSig(1:length(inputSig)-11)];
xa = xd + 1i*xh;

outputArg1 = xa;
end