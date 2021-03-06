fs = 3882;

t = 1/fs : 1/fs : 1;

bb = chirp(t,0,1,fs/2);

high = [bb(1:length(bb)/2).*0.3 bb(length(bb)/2+1:end)];
low = [bb(1:length(bb)/2) bb(length(bb)/2+1:end).*0.3];

analytic_bb_h = hilbert(high);
analytic_bb_l = hilbert(low);
analytic_bb = hilbert(bb);


re = real(analytic_bb);
im = imag(analytic_bb);

re_h = real(analytic_bb_h);
im_h = imag(analytic_bb_h);


re_l = real(analytic_bb_l);
im_l = imag(analytic_bb_l);


re_de = re(2:1:length(analytic_bb));
re_de = [re_de 0];

im_de = im(2:1:length(analytic_bb));
im_de = [im_de 0];


re_de_h = re_h(2:1:length(analytic_bb_h));
re_de_h = [re_de_h 0];

im_de_h = im_h(2:1:length(analytic_bb_h));
im_de_h = [im_de_h 0];

re_de_l = re_l(2:1:length(analytic_bb_l));
re_de_l = [re_de_l 0];

im_de_l = im_l(2:1:length(analytic_bb_l));
im_de_l = [im_de_l 0];

f = asin(-(im_de .* re) + re_de .* im);
f_l = asin(-(im_de_l .* re_l) + re_de_l .* im_l);
f_h = asin(-(im_de_h .* re_h) + re_de_h .* im_h);
bb_f_vec =  fs*(0:(length(bb))-1)/length(bb) ;


figure(1)

subplot(3,2,1)
plot(t,f, t, 0.2* bb)
yline(-0.5);

xlabel('2*pi*f in pi') 
ylabel('x(n) & y(n)') 
title('Antwort auf Frequenzsweep')

subplot(3,2,2)
plot(bb_f_vec, db(abs(fft(bb))))
ylim([0,40]);
xlabel('2*pi*f in pi') 
ylabel('x(n) & y(n)') 
title('Antwort auf Frequenzsweep')

subplot(3,2,3)
plot(t,f_h, t, 0.2* high)
yline(-0.5);
subplot(3,2,4)
plot(bb_f_vec, db(abs(fft(high))))
ylim([0,40]);

subplot(3,2,5)
plot(t,f_l, t, 0.2* low)
yline(-0.5);
subplot(3,2,6)
plot(bb_f_vec, db(abs(fft(low))))
ylim([0,40]);