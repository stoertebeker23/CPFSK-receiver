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

%modulation_factor = delta_F * 2 * T_a;
%n = 1:1:0.1/T_a;
%cpfsk_sig = amp * sin(2 * pi * fT * n * T_a + modulation_factor * pi * Phi_iT(n) + phi_0);
%ylim([-1.5 1.5])
%fvec = 1/T_a*(0:(length(cpfsk_sig))-1)/length(cpfsk_sig);
%spec_sig = abs(fft(cpfsk_sig));

%bandpass_signal = dlmread('y_cpfsk_sig.mat');
%[bandpass_signal, fswav] = audioread('CPFSK_modulate_text_ADDA8M12_20Apr21_Bec_Fra.wav');
[bandpass_signal, fswav] = audioread('CPFSK_modulate_text_ADDA8M12.wav');
figure(4)
plot(bandpass_signal)
bp_f_vec = 2019200*(0:(length(bandpass_signal))-1)/length(bandpass_signal);
bandpass_fft = abs(fft(bandpass_signal));
subplot(2,1,1)
stem(bp_f_vec, (bandpass_fft .* bandpass_fft));
subplot(2,1,2)
plot(bandpass_signal);


f_a_high = 2019200;
T_a_high = 1/f_a_high;

%neu = cpfsk_sig(1:T_a_neu/T_a:0.1/T_a);


filtered_bp = bandpass(bandpass_signal,[3800 5800],f_a_high);
bp_f_vec_1 = 1/T_a_high*(0:(length(filtered_bp))-1)/length(filtered_bp);
filtered_fft = abs(fft(filtered_bp));



f_a_low = 3832;
T_a_low = 1 / f_a_low;

bb = filtered_bp(1:round(T_a_low/T_a_high):end);
bb_f_vec =  1/T_a_low*(0:(length(bb))-1)/length(bb) ;
bb_fft = abs(fft(bb));

%%% Plotting
figure(1)

subplot(2,3,1)
stem(bp_f_vec, (bandpass_fft.*bandpass_fft));
subplot(2,3,4)
plot(bp_f_vec, db(bandpass_fft.*bandpass_fft));

subplot(2,3,2)
stem(bp_f_vec_1, (filtered_fft.*filtered_fft));
subplot(2,3,5)
plot(bp_f_vec_1, db(filtered_fft.*filtered_fft));
subplot(2,3,3)
stem(bb_f_vec, (bb_fft.*bb_fft));
subplot(2,3,6)
plot(bb_f_vec, db(bb_fft.*bb_fft));

figure(2)
t = 1:1:length(bb);
plot(t, bb)
c = bb
%% 

bb = c';
l = bb;
delayed = bb(4:1:end);
delayed = [delayed 0 0 0];

bb = bb + delayed; % delayed_b;
analytic_bb = hil(bb')';


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
plot(bb_f_vec, db(abs(fft(bb))))
ylim([-60,60])
subplot(3,2,3)
[h, w] = freqz([1 0 0 0 0 0 1],[1],f_a_low);
[i, x] = freqz([1 0 0 1],[1],f_a_low);
[j, y] = freqz([1 0 0 0 0 0 1],[1],f_a_low);
[k, z] = freqz([1 0 0 0 0 0 1],[1],f_a_low);
plot(w/pi/T_a_low,db(h), x/pi/T_a_low, db(i), y/pi/T_a_low, db(j), z/pi/T_a_low, db(k))
subplot(3,2,4)
plot(bb_f_vec,db(abs(fft(bb))./abs(fft(l)))) 

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
y