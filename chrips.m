fs = 3882;

t = 1/fs : 1/fs : 1;

bb = chirp(t,0,1,fs/2);

high = [bb(1:length(bb)/2).*0.3 bb(length(bb)/2+1:end)];
low = [bb(1:length(bb)/2) bb(length(bb)/2+1:end).*0.3];

analytic_bb_h = hilbert(high);
analytic_bb_l = hilbert(low);

re_h = real(analytic_bb_h);
im_h = imag(analytic_bb_h);


re_l = real(analytic_bb_l);
im_l = imag(analytic_bb_l);

re_de_h = re_h(2:1:length(analytic_bb_h));
re_de_h = [re_de_h 0];

im_de_h = im_h(2:1:length(analytic_bb_h));
im_de_h = [im_de_h 0];

re_de_l = re_l(2:1:length(analytic_bb_l));
re_de_l = [re_de_l 0];

im_de_l = im_l(2:1:length(analytic_bb_l));
im_de_l = [im_de_l 0];


f_l = asin(-(im_de_l .* re_l) + re_de_l .* im_l);
f_h = asin(-(im_de_h .* re_h) + re_de_h .* im_h);
bb_f_vec =  fs*(0:(length(bb))-1)/length(bb) ;


figure(1)
subplot(2,2,1)
plot(t,f_h, t, 0.2* high)
yline(-0.5);
subplot(2,2,2)
plot(bb_f_vec, db(abs(fft(high))))
ylim([0,40]);

subplot(2,2,3)
plot(t,f_l, t, 0.2* low)
yline(-0.5);
subplot(2,2,4)
plot(bb_f_vec, db(abs(fft(low))))
ylim([0,40]);