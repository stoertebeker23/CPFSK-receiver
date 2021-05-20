clear
close all

[bandpass_2M, fswav] = audioread('CPFSK_modulate_text_ADDA8M12.wav');

bp2M_f_vec = 2019200*(0:(length(bandpass_2M))-1)/length(bandpass_2M);
bandpass_2M_fft = abs(fft(bandpass_2M));

figure(1)
subplot(2,1,1)
stem(bp2M_f_vec, (bandpass_2M_fft .* bandpass_2M_fft));
title('CPFSK Spektrum')
xlabel('f[Hz]')
ylabel('|P|')
grid on
subplot(2,1,2)
plot(bandpass_2M);
title('CPFSK Signal')
% xlabel('t[s]')
grid on

%% carrier band signal

% 'red' sample frequency
f_a_high = 2019200;
T_a_high = 1/f_a_high;

[bandpass_2k, coeff, fo] = band(3800, 5800, bandpass_2M, f_a_high);
bp2k_f_vec = 1/T_a_high*(0:(length(bandpass_2k))-1)/length(bandpass_2k);
bandpass_2k_fft = abs(fft(bandpass_2k));

%% write fir bandpass coeff

filename = 'FIR_bandpass.h';
file_ID = fopen(filename, 'w');

write_coeff(file_ID, 'FIR_bandpass', coeff, length(coeff));
fclose(file_ID);

%% primary signal

% 'green' sample frequency
f_a_low = 3832;
T_a_low = 1 / f_a_low;

baseband = bandpass_2k(1:round(T_a_low/T_a_high):end);
bb_f_vec =  1/T_a_low*(0:(length(baseband))-1)/length(baseband);
baseband_fft = abs(fft(baseband));

% Plotting
figure(2)

subplot(2,3,1)
stem(bp2M_f_vec, (bandpass_2M_fft.^2));
title('CPFSK 1MHz Spektralleistung')
subplot(2,3,4)
plot(bp2M_f_vec, db(bandpass_2M_fft.^2));
title('CPFSK 1MHz Spektralleistung [dB]')

subplot(2,3,2)
stem(bp2k_f_vec, (bandpass_2k_fft.^2));
title('CPFSK 2KHz Spektralleistung')
subplot(2,3,5)
plot(bp2k_f_vec, db(bandpass_2k_fft.^2));
title('CPFSK 2KHz Spektralleistung [dB]')
subplot(2,3,3)
stem(bb_f_vec, (baseband_fft.^2));
title('CPFSK Basisbandleistung')
subplot(2,3,6)
plot(bb_f_vec, db(baseband_fft.^2));
title('CPFSK Basisband Spektralleistung [dB]')

figure(3)
t = 1:1:length(baseband);
plot(t, baseband)
title('Basisband')
tmp = baseband;
%% 
baseband = tmp';
l = baseband;
delayed = baseband(4:1:end);
delayed = [delayed 0 0 0];

baseband = baseband + delayed; % delayed_b;
analytic_bb = hil(baseband')';


a = max(analytic_bb);
analytic_bb = analytic_bb ./ a;
%fm demod

re = real(analytic_bb);
im = imag(analytic_bb);

re_de = re(2:1:length(analytic_bb));
re_de = [re_de 0];

im_de = im(2:1:length(analytic_bb));
im_de = [im_de 0];


f = asin(-(im_de .* re) + re_de .* im);
plot(t, f)
%%

figure(8)
subplot(3,2,1)
plot(bb_f_vec, db(abs(fft(l))))
ylim([-60,60]) 
subplot(3,2,2)
plot(bb_f_vec, db(abs(fft(baseband))))
ylim([-60,60])
subplot(3,2,3)
[h, w] = freqz([1 0 0 0 0 0 1],[1],f_a_low);
[i, x] = freqz([1 0 0 1],[1],f_a_low);
[j, y] = freqz([1 0 0 0 0 0 1],[1],f_a_low);
[k, z] = freqz([1 0 0 0 0 0 1],[1],f_a_low);
plot(w/pi/T_a_low,db(h), x/pi/T_a_low, db(i), y/pi/T_a_low, db(j), z/pi/T_a_low, db(k))
subplot(3,2,4)
plot(bb_f_vec,db(abs(fft(baseband))./abs(fft(l)))) 

binarized = f < -0.5;

binarized = binarized(1:0.01/T_a_low:end);
figure(5)
stem(binarized)
ylim([-0.2, 1.2])
%%

   

let = [
    'E';
    '§';
    'A';
    '{';
    'S';
    'I';
    'U';
    '$';
    'D';
    'R';
    'J';
    'N';
    'F';
    'C';
    'K';
    'T';
    'Z';
    'L';
    'W';
    'H';
    'Y';
    '}';
    'Q';
    'O';
    'B';
    'G';
    '#';
    'M';
    'X';
    'V';
    '%';
    ];

num = [
    '3';
    '§';
    '-';
    '{';
    '"';
    '8';
    '7';
    '$';
    'Ä';
    '4';
    'Ü';
    ',';
    '$';
    ':';
    '(';
    '5';
    '+';
    ')';
    '2';
    '§';
    '6';
    '}';
    '1';
    '9';
    '?';
    '§';
    '#';
    '.';
    '/';
    '=';
    '%';
    ];

y = [];
lut2 = let;
prev_r = -10;
for r = 1:length(binarized)-4
   temp = binarized(r:1:r+4);

   if temp == [1 1 1 0 0] 
      if r > 12
          if r > prev_r +14 
              prev_r = r;
              search = fliplr(binarized(r-10:2:r-1));
              ja = num2str(search);
              ja(isspace(ja)) = '';

              umsch = lut2(bin2dec(ja));
              if umsch == '%'
                  lut2 = let;
                  continue
              elseif umsch == '#'
                  lut2 = num;
                  continue
              end
              y = [y lut2(bin2dec(ja))];
          end
      end
   end
end