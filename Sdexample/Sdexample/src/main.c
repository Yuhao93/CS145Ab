/*----------------------------------------------------------------------*/
/* Foolproof FatFs sample project for AVR              (C)ChaN, 2013    */
/*----------------------------------------------------------------------*/

#include <avr/io.h>	/* Device specific declarations */

#include <stdio.h>
#include "ff.h"		/* Declarations of FatFs API */
#include "lcd.h"
#include "avr.h"
#include "asf.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */


bool check_press(int r, int c)
{
	for (int i = 0; i < 4; i++) {
		// Convert each column into an input with internal pull-up
		CLR_BIT(DDRC, i);
		SET_BIT(PORTC, i);
		
		// Convert the selected row into output ground
		// Convert all other rows into high impedance input
		if (r == i) {
			// DDR = 1, PORT = 0 => Ground
			SET_BIT(DDRC, i + 4);
			CLR_BIT(PORTC, i + 4);
			} else {
			// DDR = 0, PORT = 0 => High Impedance Input
			CLR_BIT(DDRC, i + 4);
			CLR_BIT(PORTC, i + 4);
		}
	}
	
	// Wait a few clock cycles for output voltage to update
	volatile int i;
	for (i = 0; i < 100; i++) { }
	
	return GET_BIT(PINC, c) == 0;
}

int main (void)
{
	ini_lcd();
	UINT bw;
	int state = 0;
	put_str_lcd("ready");
	while (true) {
		if (state == 0 && check_press(0, 0)) {
			FRESULT mR = f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */

			if (mR != FR_OK) {
				clr_lcd();
				char arr[17];
				sprintf(arr, "no %i", mR);
				put_str_lcd(arr);
				while (true);
			} else {
				put_str_lcd("YAY");
			}

			FRESULT res = f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);
			if (res == FR_OK) {	/* Create a file */
				put_str_lcd("writing");
				f_write(&Fil, "It works!\r\n", 11, &bw);	/* Write data to the file */
				f_close(&Fil);								/* Close the file */

				if (bw == 11) {
					clr_lcd();		/* Lights green LED if data written well */
					put_str_lcd("works");
				} else {
					put_str_lcd("error");
				}
			} else {
				clr_lcd();
				char arr[17];
				sprintf(arr, "%i", res);
				put_str_lcd(arr);
			}
			state = 1;
			put_str_lcd("done");
		}
	}

	for (;;) ;
}



/*---------------------------------------------------------*/
/* User Provided RTC Function called by FatFs module       */

DWORD get_fattime (void)
{
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}

