
short FIR_filter_sc(short FIR_delays[], const short FIR_coe[], short int N_delays, short x_n, int shift) {
	short i, y;
	int FIR_accu32=0;

	FIR_delays[N_delays-1] = x_n;	// read input sample from ADC 

	FIR_accu32	= 0;				// clear accu
	for(i=0; i < N_delays; i++)		// FIR filter routine
		FIR_accu32 += FIR_delays[N_delays-1-i] * FIR_coe[i];
	
	for(i=1; i < N_delays; i++)				
		FIR_delays[i-1] = FIR_delays[i];


	y = (short) (FIR_accu32 >>shift);
	return y;
}

