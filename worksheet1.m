clear
close all
pi = 3.14159;
T_s = 0.02;

T_a = T_s / 400;
t = 0 : T_a : 10;
d_i = square(2*pi/2/T_s*t);
stem(t,d_i)% 'or')
ylim([-2 2])

output_str = 'rürürürürü';

bistream = [0 1 0 0 1 1 1 0 0 1 1 1 1 0 0 0 0 1 1 1];
bitstream = (bistream - 0.5) * 2
sps = T_s / T_a
s_start_stop = 1.5 * sps
%%


num_integ = [0,1];
den_integ = [1, -1];
Phi_iT = filter(num_integ, den_integ, d_i);
 
plot (Phi_iT)

fT = 1/4/T_a;
delta_F = 225;
phi_0 = pi;
amp = 0.5;

modulation_factor = delta_F * 2 * T_a;
n = 1:1:5000;
cpfsk_sig = amp * sin(2 * pi * fT * n * T_a + modulation_factor * pi * Phi_iT(n) + phi_0);
figure(1)
plot(n*T_a,cpfsk_sig)
ylim([-1.5 1.5])
fvec = 1/T_a*(0:(length(cpfsk_sig))-1)/length(cpfsk_sig);
spec_sig = abs(fft(cpfsk_sig));

figure(2)
subplot(2,1,1)
stem(fvec, spec_sig.*spec_sig);
subplot(2,1,2)
plot(fvec, db(spec_sig.*spec_sig));
%pspectrum(cpfsk_sig)
%plot(n*T_a,cpfsk_sig )

%spec_sig(251)

spec_energy = sum(spec_sig.*spec_sig)/2/length(spec_sig)/2;
zweihunderteinunfuenfzich = round(length(fvec)/2/2);
b = 0.99 * spec_energy;
i = 1;
asum = 0;
while asum < (0.99 * spec_energy)
    asum = asum + spec_sig(zweihunderteinunfuenfzich+i)*spec_sig(zweihunderteinunfuenfzich+i)/length(spec_sig)/2+spec_sig(zweihunderteinunfuenfzich-i)*spec_sig(zweihunderteinunfuenfzich-i)/length(spec_sig)/2;
    i = i + 1;
end
fprintf("bandbreiti: %.0f Ohm pro Wiener Quadratklafter\n",fvec(zweihunderteinunfuenfzich+i) - fvec(zweihunderteinunfuenfzich-i))

%signed short output
signed_int = round(cpfsk_sig*32767);




