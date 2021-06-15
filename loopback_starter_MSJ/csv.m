
clear
close all
% 
FileData = load('y_cpfsk_sig.mat');
% 
x = FileData.Ax_DC_filtered;
% t = [0:1:200000];
% a = max(x)*sin(2*pi.*(1000)/2019200 * t);
% c = max(x)*sin(2*pi.*(4800)/2019200 * t);
% %a = a';
%max(x)
j = x./max(x);
c = j;

% 
% f = [c x c a x a ];
% 
% f = f';
% 
csvwrite('y_cpfsk.csv', c);

%[bandpass_signal, fswav] = audioread('CPFSK_modulate_text_ADDA8M12.wav');
%csvwrite('y_cpfsk1.csv', bandpass_signal);
% t = [0:1:200000];
% a = 5*sin(2*pi.*(4575)/2019200 * t);
% b = 5*sin(2*pi.*(5025)/2019200 * t);
% c = sin(2*pi.*(942764)/2019200 * t);
% d = sin(2*pi.*(943219)/2019200 * t);% +sin(2*pi.*200/2019200 * t) + sin(2*pi.*1700/2019200 * t);
% x = [a b ];
% x = x';
% 
% low = 4000;
% high = 5400;
% low_stop = low - 1500;
% high_stop = high + 1500;
% 
% 
% 
% 
% 
% y = chirp(t,10,500000,40000);
% 
% 
% f_a_high = 2019200;
% T_a_high = 1/f_a_high;
% 
% f_a_low = 3832;
% T_a_low = 1 / f_a_low;
% 
% 
% [N_FIR,fo,mo,w] = firpmord([low_stop low high high_stop], [0 1 0], [0.05, 0.01, 0.05],  2019200 );
% 
% b_FIR = firpm(N_FIR,fo,mo,w);
% filtered_bp1 = filter(b_FIR,[ 1 ],y);
% 
% bb1 = filtered_bp1(1:round(T_a_low/T_a_high):end);
% figure(10)
% 
% y = y';

%csvwrite('y_cpfsk.csv', x);
% 
% delta_stop = 0.01;
% delta_pass = 0.0033; 
% low = 4000;
% high = 5400;
% low_stop = low - 1500;
% high_stop = high + 1500;
% 
% 
% 
% [N_FIR,fo,mo,w] = firpmord([low_stop low high high_stop], [0 1 0], [0.05, 0.01, 0.05],  2019200 );
% 
% %b_FIR = firpm(N_FIR,fo,mo,w);
%short_filt = round(b_FIR .* (32767 * 2)); 

%freqz(b_FIR,1, 20000, 2019200, 'whole')
%csvwrite('filter.csv', short_filt);