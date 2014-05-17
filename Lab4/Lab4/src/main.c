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
#include <lcd.h>
#include <stdio.h>
#include <avr/wdt.h>



int get_numberPres(bool buttonPreSta[]);
bool check_press(int r, int c);



int get_numberPres(bool buttonPreSta[])
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(check_press(i, j) && !buttonPreSta[i * 4 + j])
			{
				return (i * 3 + j + 1);
			}
		}
	}
	if(check_press(3,1) && !buttonPreSta[3 * 4 + 1])
	{
		return 0;
	}
	
	return -1;
}

bool check_press(int c, int r)
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
unsigned int readVolatage()
{
	SET_BIT(ADCSRA, ADSC);
	while(GET_BIT(ADCSRA,ADIF) == 0 );
	SET_BIT(ADCSRA,ADIF);
	return ADC;
}
unsigned int getWhole(float num)
{
	int n = (int) (num *100.0f);
	return n/100;
}
unsigned int getDecimal(float num)
{
	
	int n = (int)(num*100.0f);
	return n%100;
}
int main (void)
{
	board_init();
	ini_lcd();
	bool buttonPreSta[16];
	for (int i = 0; i < 16; i++)
	{
		buttonPreSta[i] = false;
	}
	
	clr_lcd();
	SET_BIT(ADMUX, REFS0);
	SET_BIT(ADCSRA,ADEN);//when we want to set a conversion we set ADSC to one
	//we then wait for ADEF to be one.
	
	float min = 100;
	float max = -100;
	float sum = 0;
	float inst = 0;
	int measCount = 0;
	int state = 0;
	put_str_lcd("cheese");
	while(true)
	{
		
		switch(state) {
			case 0:
			if(get_numberPres(buttonPreSta)==1)
			{
				state = 1;
			}
			break;
			case 1:
				wait_avr(500);
				inst =  5.0f*((float) readVolatage())/1024.0f;
				measCount++;
				if (max < inst) {
					max = inst;
				}
				if (min > inst) {
					min = inst;
				}
				sum += inst;
				if(get_numberPres(buttonPreSta)==2)
				{
					state = 2;
				}
				else if(get_numberPres(buttonPreSta)==3)
				{
					state = 0;
					inst=0;
					max = -100;
					min = 100;
					sum = 0;
					measCount = 0;
					
				}
			break;
			case 2:
			if(get_numberPres(buttonPreSta)==1)
			{
				state = 1;
			}
			else if(get_numberPres(buttonPreSta)==3)
			{
				state = 0;
				inst=0;
				max = -100;
				min = 100;
				sum = 0;
				measCount = 0;
				
			}
			break;
		}
		//char buffer[17];
		//sprintf(buffer,"%2i.%02i",getWhole(inst),getDecimal(inst));
		//pos_lcd(0,0);
		//put_str_lcd(buffer);
		if (state == 0) {
			pos_lcd(0,0);
			put_str_lcd("1:----V 2:----V ");
			pos_lcd(1,0);
			put_str_lcd("3:----V 4:----V ");
		} else {
			
			
			char buffer1[100];
			char buffer2[100];
			sprintf(buffer1, "1:%1i.%02iV 2:%1i.%02iV ",getWhole(inst),getDecimal(inst),getWhole(max),getDecimal(max));
			sprintf(buffer2, "3:%1i.%02iV 4:%1i.%02iV ",getWhole(min),getDecimal(min),getWhole(sum/measCount),getDecimal(sum/measCount));
			pos_lcd(0,0);
			put_str_lcd(buffer1);
			pos_lcd(1,0);
			put_str_lcd(buffer2);
		}
		
		
		
		
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j< 4; j++) {
				buttonPreSta[i*4+j] = check_press(i,j);
			}
		}
	}
}
