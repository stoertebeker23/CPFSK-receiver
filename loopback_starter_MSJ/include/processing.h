
extern short cntr;
extern short subcntr;
extern short * out;
extern long int result;
extern short hp_result;
extern short result_short;
extern int cntrlol;
extern int cntrlel;
extern short delayed_sample;
extern short i;

extern const short * bp_filter[]; 

extern short *delay_iter;
extern short * delay_line;
extern short *rotating_rw;

extern FILE *fid_OUT, *fid_OUT2, *fid_OUT1, *fid_OUT3;

void process_sample(short value);
void debug_init();
