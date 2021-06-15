clear
close all

lol1 = readmatrix('../testsignals/y_cpfsk.csv');
figure(100)
plot(lol1)
data0 = readmatrix('../results/dec_bandpass.csv');
data3 = readmatrix('../results/demodulator.csv');
data = readmatrix('../results/combfilter.csv');

data_time = readmatrix('../results/times.csv');

data2 = complex(data(:,1),imag(data(:,2)));

y0 = fftshift(fft(data2)); 
n = length(data2);
power0 = abs(y0).^2/n;  
f0 = (-n/2:n/2-1)*(3832/n);

y2 = fftshift(fft(data0)); 
n2 = length(data0);
power2 = abs(y2).^2/n2;  
f2 = (-n2/2:n2/2-1)*(3832/n2);

ydroelf = fftshift(fft(lol1));
ndroelf = length(lol1);
powerdroelf = abs(ydroelf).^2/ndroelf;
fdroelf = (-ndroelf/2:ndroelf/2-1)*(2019200/ndroelf);

figure(3)
subplot(3,2,1)
plot(data0)
title("Ausgang Dezimationsstufe")

subplot(3,2,2)
plot(f2, db(power2))
title("Spectrum Dezimationsstufe")

subplot(3,2,3)
plot(data2)
title("Ausgang Kammfilter")
subplot(3,2,4)
plot(f0, db(power0))
title("Spektrum komplexes Kammfilter")

subplot(3,2,5)
plot(data3)
title("Ausgang Demodulator")
subplot(3,2,6)
binarized = data3 > 0;
plot(binarized)
title("Ausgang Demodulator binarisiert")
ylim([-0.2 1.2])

figure(6)
subplot(1,2,1)
plot(lol1)
subplot(1,2,2)
plot(fdroelf, db(powerdroelf))


b1 = 1/30*[ 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1];
lol = filter(b1, [1], data3);
figure(1)
subplot(2,2,1)
plot(data3)
subplot(2,2,2)
plot(lol)

subplot(2,2,4)
b = lol < 0;
plot(b)
ylim([-0.2, 1.2])


figure(23)
subplot(1,2,1)
plot(data_time(1:1:527))
subplot(1,2,2)
plot(data_time(528:1:end))