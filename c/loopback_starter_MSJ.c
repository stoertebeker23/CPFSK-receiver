/***********************************************************************
  loopback_starter_MSJ.c

  author    Svg 25-Nov-2019

  compiler  CCS 5.5 (Version: 5.5.0.00077)

  Starter Version for Master IK MSJ-Project in SS2020

  Corrections/extensions
    20-Apr-20   ANSI C directory provided for simulation using PC

***********************************************************************/

/***********************************************************************

  loopback.c - D.Module2.C6747 + ADDA8M12 Test Program

  author    A.Klemenz
  company   D.SignT
  revision  1.0
  date      2012-02-14

  hardware  D.Module.ADDA8M12
            D.Module2.C6747

  compiler  CCS 3.3

  history   1.0 initial release 2012-02-14 by AK

  Note: The maximum usable ADC sampling frequency with a D.Module2.C6747
        is 4MHz. At higher sampling frequencies the ADC EDMA will not
        complete before the next interrupt is generated.

        EDMA stalls occur if both ADC and DAC EDMAs are active. This
        limits the max. usable sampling frequency for this loopback
        program to 2MHz
***********************************************************************/

/***********************************************************************
  includes
***********************************************************************/
#include "processor.h" // control whether we have ANSI_C or HARDWARE_ADDA12M8_C6747 version

#ifdef USE_HARDWARE_ADDA8M12_C6747
#include "..\..\dmodule\dm2c6747.h"
//#include "..\..\dmodule\dm2adda8m12.h"
// Siehe A. Klemenz Email von 24-Jun-19
#include "..\..\dmodule\dm2adda8m12_24Jun19.h"
#endif

#include <string.h>

#ifdef USE_HARDWARE_ADDA8M12_C6747
#include <ti/pspiom/cslr/soc_C6747.h>    /* Peripheral Registers      */
#include <ti/pspiom/cslr/cslr_edma3cc.h> /* EDMA Channel Controller   */
#include <ti/pspiom/cslr/cslr_edma3tc.h> /* EDMA Transfer Controller  */
#endif

/***********************************************************************
  size of the ADC and DAC buffer (number of samples)
***********************************************************************/
#define ADDA8M12_BUFSIZE  	2048

/***********************************************************************
  define for VT100 terminal output
***********************************************************************/
#ifdef USE_HARDWARE_ADDA8M12_C6747
#define STR_VALUE(x)    #x
#define VT100_CLR       "\x1B[2J\x1B[H"
#define VT100_ATTRIB(x) "\x1B["STR_VALUE(x)"m"
#define VT100_POS(x,y)  "\x1B["STR_VALUE(y)";"STR_VALUE(x)"H"

#define VT100_ATTRIB_BOLD   1
#define VT100_ATTRIB_NORM   0
#endif

/***********************************************************************
  global variables D.signT
***********************************************************************/
int ADDA8M12_adc[2][ADDA8M12_BUFSIZE];	// ping-pong acquisition buffer for ADC
int ADDA8M12_dac[2][ADDA8M12_BUFSIZE];  // ping-pong buffer for DAC output

volatile int ADDA8M12_bufferReady = -1; // ping(0) or pong(1) buffer ready for processing
                                        // -1: no buffer ready
#ifdef USE_HARDWARE_ADDA8M12_C6747
/** EDMA overlay                                                      */
CSL_Edma3ccRegsOvly edma3cc0Regs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;
#endif

#include <stdio.h>	// for printf
#include <math.h>	// for sin/cos
#include <stdlib.h>	// for FILE*

// ###################################################################
//------------- BEGIN OF Svg declaration section -----------------------
// ###################################################################

//--------------------------------------------------------------------
// unions
//--------------------------------------------------------------------

#define LEFT 0
#define RIGHT 1
union {
	unsigned int both;
	short channel[2];
} ADDA8M12_ADC_data; // union for ADC data

union {
	unsigned int both;
	short channel[2];
} ADDA8M12_DAC_data; // union for DAC data

//--------------------------------------------------------------------
// prototypes
//--------------------------------------------------------------------
void process_one_sample();

//--------------------------------------------------------------------
//  global variables
//--------------------------------------------------------------------
	short 	indx;
	short 	left_in, right_in;
	int 	tempbuffer[2][ADDA8M12_BUFSIZE];	// tempbuffer buffer for ADC
	short 	left_channel_buffer[ADDA8M12_BUFSIZE];	// left_channel_buffer for ADC
	short 	right_channel_buffer[ADDA8M12_BUFSIZE];	// right_channel_buffer for ADC
	float 	PI;
#ifdef USE_MSVC_ANSI_C_SIM
    short count_blks = 0; // counter for number of EDMA blocks
    int ok; // return value of fscanf
    FILE* fid_ADC_data, * fid_DAC_data;
    char* ADC_input_file, * DAC_output_file;
    float left_in_float;

#endif

// ###################################################################
//------------- END OF Svg declaration section -----------------------
// ###################################################################


/*******************************************************************//**
 @brief     EDMA Complete Interrupt
***********************************************************************/
#ifdef USE_HARDWARE_ADDA8M12_C6747
interrupt void edma_int (void)
{
    /*******************************************************************
      locals
    *******************************************************************/
	static int32_t block = 0;

    /*******************************************************************
	  if ADC EDMA complete
    *******************************************************************/
    if (edma3cc0Regs->IPR & (1 << CSL_EDMA3_CHA_GPIO_BNKINT2))
    {
		/* clear event */
        edma3cc0Regs->ICR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);

		/* flag new block as ready, toggle block */
        ADDA8M12_bufferReady = block;
		block ^= 1;
	}

    /*******************************************************************
	  process other EDMA interrupt events (if enabled)
    *******************************************************************/
}
#endif

#ifdef USE_MSVC_ANSI_C_SIM
// 20-Apr-20:
// Das ping-pong buffer system wurde in ANSI C nicht zu 100 % nachgebaut, da dies nicht erforderlich ist.
// Die Variable int ADDA8M12_adc[2][indx] ist zurzeit auf int ADDA8M12_adc[0..1][0..2047] eingestellt.

// Es gilt:
// ping buffer : ADDA8M12_adc[0][0..2047]
// pong buffer : ADDA8M12_adc[1][0..2047]

// Die Variable ADDA8M12_bufferReady entscheidet (0 oder 1), ob ping oder pong verwendet wird

// Das Array-Element "int ADDA8M12_adc[0][0]" (32 bit( enth�lt sowohl die Samples des linken als auch des rechten Kanals.
// linker und rechter Kanal : jeweils 16 bit pro Abtastwert.
// in diesen 32 Bit gilt : untere 16 Bits : linker Kanal, obere 16 Bits : rechter Kanal.
// Bsp.: 0xABCD1234, links = 0x1234, rechts = 0xABCD

// Wir verwenden immer nur den ping Buffer [0], also ist ADDA8M12_bufferReady immer gleich 0.

// Das aus der Datei des CPFSK-Signals eingelesene Signal (Datentyp float) wird auf einen 16 bit short int konvertiert.
// Der ADC liest immer 32 bit links und rechts in einem Lesezyklus, untere 16 bits LINKS, obere 16 bits RECHTS.
void edma_int(void)
{
    ADDA8M12_bufferReady = 0;
	count_blks++;
	if (count_blks % 2 == 0)
		printf("ADDA8M12_BUFSIZE = %d --> block number %d read\n", ADDA8M12_BUFSIZE, count_blks);

	for (indx=0; indx<ADDA8M12_BUFSIZE; indx++)
	{
// linken Kanal als float einlesen, auf "short int" 16 bit format bringen
        ok = fscanf(fid_ADC_data, "%f", &left_in_float);
// Unwandeln des ADC input samples in einen "short", speichern dieser 16 Bits (left channel data) in int ADDA8M12_adc[0..1][indx] 
// Die oberen 16 Bits enthalten also Nullen, der rechte Kanal enth�lt also nur Nullen
        ADDA8M12_adc[ADDA8M12_bufferReady][indx] = (short)(left_in_float * 32767.f);
		if (ok != 1) {
			printf("stop, EOF reached...\n");
			fclose(fid_ADC_data);
			fclose(fid_DAC_data);
// Die Ausgabedatei (DAC Signal) kann man sich bei Bedarf auf der Konsole ausgeben lssen ...
//          system("type ADDA8M12_DAC_data.mat");
			system("pause");
			exit(0);
		}
	} // for (indx...)
}
#endif

/***********************************************************************

  Main Program

***********************************************************************/
int main (void)
{

    /*******************************************************************
      local variables
    *******************************************************************/
#ifdef USE_HARDWARE_ADDA8M12_C6747
    T_Handle *h_uart;
#endif
	PI = 4 * atan(1);

#ifdef USE_MSVC_ANSI_C_SIM
// Der ADC "liest" aus dieser Datei
    ADC_input_file  = "y_cpfsk_sig.mat"; // enthält floats, nur ein Kanal
// Der DAC "schreibt" in diese Datei
    DAC_output_file = "ADDA8M12_DAC_data.mat";

// Dateien oeffnen, existiert ADC-Datei ueberhaupt?
    fid_ADC_data = fopen(ADC_input_file,  "r");
    if (fid_ADC_data == NULL)
    {   printf("Input file > y_cpfsk_sig.mat < file not found\n");
        system("pause");
        exit(0);
    }
    fid_DAC_data = fopen(DAC_output_file, "w");
#endif

#ifdef USE_HARDWARE_ADDA8M12_C6747
    /***************************************************************//**
      - configure Bus Interface CS0 (DSP CE4) to 1-3-1 timing
        (setup-strobe-hold = 10-30-10ns @ 100MHz default BUSCLK)
      - initialize DSP module
      - reset peripheral boards
    *******************************************************************/
    DM2_hwSettings.emifa_ce4cfg = 0x04322195;
    DM2_init(0);
    DM2_busConfig (DM2_BUSRESOUT);

    /*******************************************************************
      - print banner
    *******************************************************************/
    h_uart = DM2_uartOpen(0);
    DM2_uartWriteStr (
        h_uart,
        VT100_CLR
        VT100_ATTRIB(VT100_ATTRIB_BOLD)
        ".---------------------------------------------------------.\r\n"
        "| D.Module2.C6747 + D.Module2.ADDA8M12      EDMA Loopback |\r\n"
        "|                                                         |\r\n"
        "| (c) D.SignT 2012                          www.dsignt.de |\r\n"
    	"| (c) HAW Hamburg U. Sauvagerd Wetterdaten KW 2019        |\r\n"
        "'---------------------------------------------------------'\r\n\n"
        VT100_ATTRIB(VT100_ATTRIB_NORM)
        ,DM2_UART_NOTIMEOUT );
#endif

#ifdef USE_HARDWARE_ADDA8M12_C6747
    /*******************************************************************
      take ADDA8M12 out of reset,
      configure bus interface CS0 memory area to 32-bit access
      clear DAC output
    *******************************************************************/
    DM2_busConfig (DM2_BUSCS0_32);
	ADDA8M12->dac = 0;

    /*******************************************************************
	  configure ADDA8M12
	   - ADC internal 2MHz clock
	   - DAC update synchronous to ADC
       - do not yet enable interrupts
    *******************************************************************/
	ADDA8M12->config = ADDA8M12_ADCCLKINT
	                 | ADDA8M12_INTCLK2M
					 | ADDA8M12_DACCLKADC;

    /*******************************************************************
	  map INT0 (ADC) and INT1 (DAC) to GPIO bank 0 and 5 as EDMA trigger
    *******************************************************************/
    DM2_intMap (DM2_INTEVT_INT0, DM2_INTGP2_0);
    DM2_intMap (DM2_INTEVT_INT1, DM2_INTGP5_10);

    /*******************************************************************
	  Interrupt Initialization, enable EDMA  interrupt
    *******************************************************************/
    DM2_intInit (&DM2_intServiceTable);
    DM2_intSel (15, DM2_EVT_EDMA3_CC0_INT1);
    DM2_intInstFunc (15, edma_int);
    DM2_intStart();

    /*******************************************************************
	  clear data acquisition buffers
    *******************************************************************/
	memset(ADDA8M12_adc, 0, sizeof(ADDA8M12_adc));
	memset(ADDA8M12_dac, 0, sizeof(ADDA8M12_dac));


    /**************************************************************//**
      EDMA initialization
    ******************************************************************/
    /**************************************************************//**
      - power on EDMA controller
    ******************************************************************/
	DM2_powerConfig(0, 0, DM2_PWR_ENABLE);
	DM2_powerConfig(0, 1, DM2_PWR_ENABLE);
	DM2_powerConfig(0, 2, DM2_PWR_ENABLE);

    /**************************************************************//**
      - clear pending EDMA events  (only required for program restart)
    ******************************************************************/
 	edma3cc0Regs->EECR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);
 	edma3cc0Regs->ECR  = (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);
 	edma3cc0Regs->SECR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);

 	edma3cc0Regs->EECR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT5);
 	edma3cc0Regs->ECR  = (1 << CSL_EDMA3_CHA_GPIO_BNKINT5);
 	edma3cc0Regs->SECR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT5);

    /**************************************************************//**
      - map ADC EDMA channel to queue 0 (TC0) (default
        map DAC EDMA channel to queue 1 (TC1) to avoid conflicts
    ******************************************************************/
 	edma3cc0Regs->DMAQNUM[CSL_EDMA3_CHA_GPIO_BNKINT2 / 8] &=
 		~(1 << (4 * (CSL_EDMA3_CHA_GPIO_BNKINT2 & 7)));

 	edma3cc0Regs->DMAQNUM[CSL_EDMA3_CHA_GPIO_BNKINT5 / 8] |=
 		(1 << (4 * (CSL_EDMA3_CHA_GPIO_BNKINT5 & 7)));


    /**************************************************************//**
      - DAC EDMA, triggered by BANK5 event
        using Parameter Set 102 and 103 for reload
        - A-synchronized transfers
        - ACNT = 4 bytes (32-bit) = DAC-Channel 1 and 2
        - BCNT = ADDA8M12_BUFSIZE
        - CCNT = 1
        - SRC B IDX = 4  increment pointer in source buffer 4 bytes
                         (32-bit) with each transfer
        - DST B IDX = 0  fixed address for destination (DAC)
        - DMA is complete after CCNT*BCNT*ACNT bytes have been
          transferred -> reload
        - enable BANK5 event
    ******************************************************************/
	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].OPT =
	edma3cc0Regs->PARAMSET[102].OPT =
	edma3cc0Regs->PARAMSET[103].OPT =
	  	CSL_FMK  (EDMA3CC_OPT_PRIVID, 0)
      | CSL_FMKT (EDMA3CC_OPT_ITCCHEN, DISABLE)
      | CSL_FMKT (EDMA3CC_OPT_TCCHEN, DISABLE)
      | CSL_FMKT (EDMA3CC_OPT_ITCINTEN, DISABLE)
      | CSL_FMKT (EDMA3CC_OPT_TCINTEN, ENABLE)
      | CSL_FMK  (EDMA3CC_OPT_TCC, CSL_EDMA3_CHA_GPIO_BNKINT5)
      | CSL_FMKT (EDMA3CC_OPT_TCCMOD, NORMAL)
      | CSL_FMKT (EDMA3CC_OPT_FWID, 32BIT) // don't care
      | CSL_FMKT (EDMA3CC_OPT_STATIC, NORMAL)
      | CSL_FMKT (EDMA3CC_OPT_SYNCDIM, ASYNC)
      | CSL_FMKT (EDMA3CC_OPT_DAM, INCR)
      | CSL_FMKT (EDMA3CC_OPT_SAM, INCR);

    edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].DST =
	edma3cc0Regs->PARAMSET[102].DST =
	edma3cc0Regs->PARAMSET[103].DST =
        (uint32_t)&ADDA8M12->dac;

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].SRC =
	edma3cc0Regs->PARAMSET[103].SRC =
 		(uint32_t)&ADDA8M12_dac[0][0];
	edma3cc0Regs->PARAMSET[102].SRC =
 		(uint32_t)&ADDA8M12_dac[1][0];

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].A_B_CNT =
	edma3cc0Regs->PARAMSET[102].A_B_CNT =
	edma3cc0Regs->PARAMSET[103].A_B_CNT =
   	  	CSL_FMK (EDMA3CC_A_B_CNT_BCNT, ADDA8M12_BUFSIZE)
 	  | CSL_FMK (EDMA3CC_A_B_CNT_ACNT, 4);

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].CCNT =
 	edma3cc0Regs->PARAMSET[102].CCNT =
	edma3cc0Regs->PARAMSET[103].CCNT =
		1;

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].SRC_DST_BIDX =
	edma3cc0Regs->PARAMSET[102].SRC_DST_BIDX =
	edma3cc0Regs->PARAMSET[103].SRC_DST_BIDX =
  	  	CSL_FMK (EDMA3CC_SRC_DST_BIDX_SRCBIDX, 4)
  	  | CSL_FMK (EDMA3CC_SRC_DST_BIDX_DSTBIDX, 0);

   	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].SRC_DST_CIDX =
	edma3cc0Regs->PARAMSET[102].SRC_DST_CIDX =
	edma3cc0Regs->PARAMSET[103].SRC_DST_CIDX =
 	  	CSL_FMK (EDMA3CC_SRC_DST_CIDX_SRCCIDX, 0)
	  | CSL_FMK (EDMA3CC_SRC_DST_CIDX_DSTCIDX, 0);

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT5].LINK_BCNTRLD =
	edma3cc0Regs->PARAMSET[103].LINK_BCNTRLD =
	    CSL_FMK (EDMA3CC_LINK_BCNTRLD_LINK, ((uint32_t)&edma3cc0Regs->PARAMSET[102] & 0xFFFF))
  	  | CSL_FMK (EDMA3CC_LINK_BCNTRLD_BCNTRLD, ADDA8M12_BUFSIZE);
	edma3cc0Regs->PARAMSET[102].LINK_BCNTRLD =
	    CSL_FMK (EDMA3CC_LINK_BCNTRLD_LINK, ((uint32_t)&edma3cc0Regs->PARAMSET[103] & 0xFFFF))
  	  | CSL_FMK (EDMA3CC_LINK_BCNTRLD_BCNTRLD, ADDA8M12_BUFSIZE);

 	edma3cc0Regs->EESR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT5);


    /**************************************************************//**
      - ADC EDMA, triggered by BANK2 event
        using Parameter Set 100 and 101 for reload
        - A-synchronized transfers (ADC generates trigger if fifo is full
          i.e. if 8 words are available)
        - ACNT = 8 words of 4 bytes = ADC-Channel 1 and 2 fifo
        - BCNT = ADDA8M12_BUFSIZE/8
        - CCNT = 1
        - SRC B IDX = 0  fixed address for source (ADC)
        - DST B IDX = 4  increment pointer in destination buffer by
                         8 words of 4 bytes with each transfer
        - DMA is complete after CCNT*BCNT*ACNT bytes have been
          transferred -> parameter reload and EDMA complete interrupt
        - enable Bank2 event
    ******************************************************************/
	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].OPT =
	edma3cc0Regs->PARAMSET[100].OPT =
	edma3cc0Regs->PARAMSET[101].OPT =
	  	CSL_FMK  (EDMA3CC_OPT_PRIVID, 0)
      | CSL_FMKT (EDMA3CC_OPT_ITCCHEN, DISABLE)
      | CSL_FMKT (EDMA3CC_OPT_TCCHEN, DISABLE)
      | CSL_FMKT (EDMA3CC_OPT_ITCINTEN, DISABLE)
      | CSL_FMKT (EDMA3CC_OPT_TCINTEN, ENABLE)
      | CSL_FMK  (EDMA3CC_OPT_TCC, CSL_EDMA3_CHA_GPIO_BNKINT2)
      | CSL_FMKT (EDMA3CC_OPT_TCCMOD, NORMAL)
      | CSL_FMKT (EDMA3CC_OPT_FWID, 32BIT) // don't care
      | CSL_FMKT (EDMA3CC_OPT_STATIC, NORMAL)
      | CSL_FMKT (EDMA3CC_OPT_SYNCDIM, ASYNC)
      | CSL_FMKT (EDMA3CC_OPT_DAM, INCR)
      | CSL_FMKT (EDMA3CC_OPT_SAM, INCR);

    edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].SRC =
	edma3cc0Regs->PARAMSET[100].SRC =
	edma3cc0Regs->PARAMSET[101].SRC =
        (uint32_t)&ADDA8M12->adc;

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].DST =
	edma3cc0Regs->PARAMSET[101].DST =
 		(uint32_t)&ADDA8M12_adc[1][0];
	edma3cc0Regs->PARAMSET[100].DST =
 		(uint32_t)&ADDA8M12_adc[0][0];

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].A_B_CNT =
	edma3cc0Regs->PARAMSET[100].A_B_CNT =
	edma3cc0Regs->PARAMSET[101].A_B_CNT =
   	  	CSL_FMK (EDMA3CC_A_B_CNT_BCNT, ADDA8M12_BUFSIZE/8)
 	  | CSL_FMK (EDMA3CC_A_B_CNT_ACNT, 4*8);

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].CCNT =
 	edma3cc0Regs->PARAMSET[100].CCNT =
	edma3cc0Regs->PARAMSET[101].CCNT =
 		1;

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].SRC_DST_BIDX =
	edma3cc0Regs->PARAMSET[100].SRC_DST_BIDX =
	edma3cc0Regs->PARAMSET[101].SRC_DST_BIDX =
  	  	CSL_FMK (EDMA3CC_SRC_DST_BIDX_SRCBIDX, 0)
  	  | CSL_FMK (EDMA3CC_SRC_DST_BIDX_DSTBIDX, 4*8);

   	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].SRC_DST_CIDX =
	edma3cc0Regs->PARAMSET[100].SRC_DST_CIDX =
	edma3cc0Regs->PARAMSET[101].SRC_DST_CIDX =
 	  	CSL_FMK (EDMA3CC_SRC_DST_CIDX_SRCCIDX, 0)
	  | CSL_FMK (EDMA3CC_SRC_DST_CIDX_DSTCIDX, 0);

 	edma3cc0Regs->PARAMSET[CSL_EDMA3_CHA_GPIO_BNKINT2].LINK_BCNTRLD =
	edma3cc0Regs->PARAMSET[100].LINK_BCNTRLD =
	    CSL_FMK (EDMA3CC_LINK_BCNTRLD_LINK, ((uint32_t)&edma3cc0Regs->PARAMSET[101] & 0xFFFF))
  	  | CSL_FMK (EDMA3CC_LINK_BCNTRLD_BCNTRLD, ADDA8M12_BUFSIZE/8);
	edma3cc0Regs->PARAMSET[101].LINK_BCNTRLD =
	    CSL_FMK (EDMA3CC_LINK_BCNTRLD_LINK, ((uint32_t)&edma3cc0Regs->PARAMSET[100] & 0xFFFF))
  	  | CSL_FMK (EDMA3CC_LINK_BCNTRLD_BCNTRLD, ADDA8M12_BUFSIZE/8);

 	edma3cc0Regs->EESR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);

    /*******************************************************************
      enable EDMA complete interrupt for ADC
    *******************************************************************/
    edma3cc0Regs->DRA[1].DRAE |= (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);
 	edma3cc0Regs->IESR = (1 << CSL_EDMA3_CHA_GPIO_BNKINT2);



    /*******************************************************************
      configure and calibrate ADC
    *******************************************************************/
    ADDA8M12_adcConfig();

    /*******************************************************************
	  start data acquisition
	  by enabling ADDA8M12 interrupts
    *******************************************************************/
	ADDA8M12_bufferReady = -1;

// Set internal or external clock mode
/* A, Klemenz 24-Mai-19
	Die Abtastfrequenz wird in main() eingestellt:
	     ADDA8M12->config = ADDA8M12_ADCCLKINT
	                      | ADDA8M12_INTCLK2M  // ADC Abtasttakt
	                      | ADDA8M12_DACCLKADC // DAC Abtasttakt
	                      | ADDA8M12_INT0ADC
	                      | ADDA8M12_INT1DAC;
	F�r den ADC haben Sie die Optionen
	ADDA8M12_INTCLK1M       1MHz
	ADDA8M12_INTCLK2M       2MHz
	ADDA8M12_INTCLK4M       4MHz, beim 6747 aber nur m�glich wenn
	                               der DAC nicht benutzt wird,
	                               sonst kommt die EDMA nicht mehr mit
	ADDA8M12_INTCLK8M       8MHz, nicht m�glich beim 6747
	ADDA8M12_ADCCLKEXT      �ber die EXTCLK Buchse eingespeist
	ADDA8M12_ADCCLKGP0      Takt an GPIO0
	ADDA8M12_ADCCLKGP1      Takt an GPIO1
*/
// EXT CLOCK
	ADDA8M12->config = ADDA8M12_ADCCLKINT
	                 | ADDA8M12_INTCLK2M //ADDA8M12_INTCLK2M // EXT_CLK: hier muss spaeter ADDA8M12_ADCCLKEXT stehen
					 | ADDA8M12_DACCLKADC
					 | ADDA8M12_INT0ADC
					 | ADDA8M12_INT1DAC;

    /*******************************************************************
	  main loop
    *******************************************************************/
    DM2_uartWriteStr (h_uart, "running...\r\n", DM2_UART_NOTIMEOUT);
#endif

// infinite for loop ...
    for (;;)
    {
#ifdef USE_MSVC_ANSI_C_SIM   // der Interrupt muss hier natürlich manuell aufgerufen werden
        edma_int(); // call ISR, read next sample
#endif
        /***************************************************************
    	 ADDA8M12_bufferReady shows which buffer is ready for processing
    	 0 (ping), 1 (pong), or -1 (none)
        ***************************************************************/
    	if (ADDA8M12_bufferReady != -1)
        {
        	/***********************************************************
             copy source (ADC) buffer to destination (DAC) buffer
        	***********************************************************/
#ifdef USE_HARDWARE_ADDA8M12_C6747
// Diese Code-Zeilen bitte erstmal nicht verwenden. Das Kopieren der Daten vom ADC zum DAC
// geschieht unten "von Hand"
//          memcpy (ADDA8M12_dac[ADDA8M12_bufferReady], ADDA8M12_adc[ADDA8M12_bufferReady], 4*ADDA8M12_BUFSIZE);
// Der tempbuffer[] enthaelt ine Kopie des ADDA8M12_adc[2][ADDA8M12_BUFSIZE]-Buffers.
// Wenn man den Prozessor an einem Haltepunkt anhaelt, laeuft die EDMA weiter und der
// ADDA8M12_adc[2][ADDA8M12_BUFSIZE] aendert seinen Inhalt.
// Der tempbuffer[] aendert sich jedoch nicht, weil der Prozessor nicht mehr laeuft, denn der Prozessor
// fuehrt ja das Kopieren nicht mehr aus.
//			memcpy (tempbuffer[ADDA8M12_bufferReady], ADDA8M12_adc[ADDA8M12_bufferReady], 4*ADDA8M12_BUFSIZE);
#endif

            for (indx=0; indx<ADDA8M12_BUFSIZE; indx++)
            {
#ifdef USE_HARDWARE_ADDA8M12_C6747
// Lesen vom EDMA ADC buffer
            	ADDA8M12_ADC_data.channel[LEFT]  = (short)(ADDA8M12_adc[ADDA8M12_bufferReady][indx]    );
            	ADDA8M12_ADC_data.channel[RIGHT] = (short)(ADDA8M12_adc[ADDA8M12_bufferReady][indx]>>16);
// Bei Bedarf: Kopieren der "int tempbuffer[2][]" - Daten nach short left_buffer[] und short right_buffer[]
//            	left_channel_buffer[indx]  = (short)(tempbuffer[ADDA8M12_bufferReady][indx]    );
//            	right_channel_buffer[indx] = (short)(tempbuffer[ADDA8M12_bufferReady][indx]>>16);

// Wenn man den linken und rechten Kanal in zwei separaten Buffern speichern will
            	left_channel_buffer[indx]  = ADDA8M12_ADC_data.channel[LEFT];
            	right_channel_buffer[indx] = ADDA8M12_ADC_data.channel[RIGHT];
// Wenn man einzelne Samples fuer ADC LINKS und RECHTS weiter verarbeiten will...
            	left_in  = ADDA8M12_ADC_data.channel[LEFT];
				right_in = ADDA8M12_ADC_data.channel[RIGHT];
#endif
#ifdef USE_MSVC_ANSI_C_SIM
// Lesen vom EDMA ADC buffer, direkt auf left_in
                left_in = ADDA8M12_adc[ADDA8M12_bufferReady][indx];
#endif

// innerhalb der for-loop kann in der Funktion "process_one_sample()" sample-basiert gearbeitet werden
                process_one_sample();

// Zuweisen der ADC Daten an DAC Daten ueber UNIONs
//				ADDA8M12_DAC_data.channel[LEFT]  = ADDA8M12_ADC_data.channel[LEFT];
//              ADDA8M12_DAC_data.channel[RIGHT] = ADDA8M12_ADC_data.channel[RIGHT];
// einzelne Samples fuer DAC LINKS und RECHTS zum DAC ..
				ADDA8M12_DAC_data.channel[LEFT]  = left_in;  // ADDA8M12_ADC_data.channel[LEFT];
                ADDA8M12_DAC_data.channel[RIGHT] = right_in; // ADDA8M12_ADC_data.channel[RIGHT];

// DAC Ausgabe : Schreiben left_in und right_in zum DAC, auch hier UNION verwenden
// Bei Bedarf: Kopieren von rechts nach links, wenn man nur einen Eingang benutzt
				ADDA8M12_DAC_data.channel[LEFT]  = ADDA8M12_ADC_data.channel[RIGHT];
                ADDA8M12_dac[ADDA8M12_bufferReady][indx] = ADDA8M12_DAC_data.both;

            } // end for (indx; ...

        	/***********************************************************
             processing is done
        	***********************************************************/
			ADDA8M12_bufferReady = -1;
        }
	} // for (;;;)
}
