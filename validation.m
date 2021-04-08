clear
close all
pi = 3.14159;
T_s = 0.02;

f_a = 2.1*11100800;
f_t = 10100800;

T_a = 1/f_a;
t = 0 : T_a : 0.1;
d_i = square(2*pi/2/T_s*t);
ylim([-2 2])


num_integ = [0,1];
den_integ = [1, -1];
Phi_iT = filter(num_integ, den_integ, d_i);
 
plot (Phi_iT)

fT = f_t;
delta_F = 225;
phi_0 = pi;
amp = 0.5;

modulation_factor = delta_F * 2 * T_a;
n = 1:1:0.1/T_a;
cpfsk_sig = amp * sin(2 * pi * fT * n * T_a + modulation_factor * pi * Phi_iT(n) + phi_0);
ylim([-1.5 1.5])
fvec = 1/T_a*(0:(length(cpfsk_sig))-1)/length(cpfsk_sig);
spec_sig = abs(fft(cpfsk_sig));


subplot(2,1,1)
stem(fvec, (spec_sig.*spec_sig));
subplot(2,1,2)
plot(fvec, db(spec_sig.*spec_sig));

f_a_neu = 2019200;
T_a_neu = 1/f_a_neu;

neu = cpfsk_sig(1:T_a_neu/T_a:0.1/T_a);
x = bandpass(neu,[3800 5800],f_a_neu);
fvec_n = 1/T_a_neu*(0:(length(x))-1)/length(x);
spec_sig_n = abs(fft(x));



fa_f = 3832;
ta_f = 1 / fa_f;

echt_neu = x(1:round(ta_f/T_a_neu):0.1/T_a_neu);
f_vec_bla =  1/ta_f*(0:(length(echt_neu))-1)/length(echt_neu) ;
spec_sig_f = abs(fft(echt_neu));

figure(1)

subplot(2,3,1)
stem(fvec, (spec_sig.*spec_sig));
subplot(2,3,4)
plot(fvec, db(spec_sig.*spec_sig));


subplot(2,3,2)
stem(fvec_n, (spec_sig_n.*spec_sig_n));
subplot(2,3,5)
plot(fvec_n, db(spec_sig_n.*spec_sig_n));
subplot(2,3,3)
stem(f_vec_bla, (spec_sig_f.*spec_sig_f));
subplot(2,3,6)
plot(f_vec_bla, db(spec_sig_f.*spec_sig_f));

