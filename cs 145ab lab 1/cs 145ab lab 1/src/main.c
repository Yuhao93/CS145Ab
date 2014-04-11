/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <avr.h>

void sleep500()
{
	volatile unsigned int i;
	for(i= 0; i<= 27778 ; i++)
	{
		
	}
	
}
void internalTimer500()
{
	TCCR0 = 5;
	for(int i = 0; i < 2; i++){
		TCNT0 = 0;
		while(true){
			unsigned char val = TCNT0;
			if(val >= 255)
			{
				break;
			}
		}
	}
}
int main (void)
{
	board_init();
	SET_BIT(DDRB,0);
	CLR_BIT(DDRB,1);
	
	for(;;)
	{
		
		if(GET_BIT(PINB,1) == 0)
		{
			
			SET_BIT(PORTB, 0);
			//sleep500();
			//wait_avr(t);
			internalTimer500();
			CLR_BIT(PORTB, 0);
			//sleep500();
			//wait_avr(t);
			internalTimer500();
		}
		else
		{
			
			CLR_BIT(PORTB,0);
		}
		
		
		}
	// Insert application code here, after the board has been initialized.
}
