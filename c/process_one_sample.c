/***********************************************************************

 process_one_sample.c

 author    Uli Sauvagerd
 date      20-Apr-20
 revision  1.0

 hardware  D.Module.ADDA8M12 +  D.Module2.C6747

 compiler  CCS 5.50

 history   1.0 initial release 20-Apr-20 by US

 allows to process ADC signal left_in sample-based

           Errors:
           Corrections:
           Comments:

***********************************************************************/

#include <stdio.h>	// for printf
#include <math.h>	// for sin/cos
#include <stdlib.h>	// for FILE*

#include "processor.h"
// Alle Variablen sind i.d.Regel global, daher über "extern" bekannt machen
extern FILE* fid_DAC_data;
extern short left_in;

// prototype
void process_one_sample();

// function body
void process_one_sample()
{
// linker Kanal wird mit 0.5 multipliziert 
    left_in /= 2;
// Hilbert Filter in ANSI C
// (...)
// FM Demodulator
// (...) Ergebnis steht in y_CPFSK_demod 
	
#ifdef USE_MSVC_ANSI_C_SIM
// Schreiben in DAC Datei, aber nur im Falle von ANSI C
    fprintf(fid_DAC_data, "%hd\n", left_in); // y_CPFSK_demody
#endif
}
