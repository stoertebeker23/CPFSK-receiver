% Design komplex filter

b_compl = [1 0 0 0 0.17502 - 0.98456i];
b = [1 0 0 0 1];

a = 1;
N = length(b)-1
%freqz(b,1, 20000, 3832)

Fc = .389;                              % Desired frequency shift
%j = complex(0,1);
%ai = b(1)*exp(j*pi*0)
%aj = b(N+1)*exp(j*pi*N)
%b_compl = b.*exp(j*Fc*pi*(0:N))

%blol = b(1)*exp(j*Fc*pi*0)
%blal = b(N+1)*exp(j*Fc*pi*N / N)
fvtool(b,a,b_compl,a)

%freqz(b_compl,1, 20000, 3832, 'whole')