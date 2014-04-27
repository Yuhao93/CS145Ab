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
#include <avr/sleep.h>
#include <avr/interrupt.h>

//#define _BV(BIT) (1<<BIT)

bool check_press(int r, int c);

/*void io_init()
{
	cli()
	board_init();
	ini_lcd();
	ini_avr();
	calendar.sec =50 ;
	calendar.min = 59;
	calendar.hour= 23;
	calendar.day=28;
	calendar.month=2;
	calendar.year=5;
	wdt_reset();
	WDTCR |= 1<<WDTOE;
	WDTCR = (1<<WDE)|(1<<WDP2)|(1<<WDP1));
	sei();
}
ISR()
{
	sleep_disable();
	clr_lcd();
	calendar.sec++;
	if(calendar.sec == 60)
	{
		calendar.min++;
		calendar.sec = 0;
	}
	if(calendar.min == 60)
	{
		
		calendar.hour++;
		calendar.min = 0;
	}
	if(calendar.hour == 24)
	{
		calendar.day++;
		calendar.hour=0;
	}
	if(calendar.day > dayTbl[calendar.month]+(isLeapYear(calendar.year) &&calendar.month ==2 ? 1 : 0))
	{
		calendar.day =1;
		calendar.month++;
	}
	if(calendar.month == 13)
	{
		calendar.year++;
		calendar.month=1;
	}
	sleep_enable();
}*/
int get_numberPres()
{
	for(int i =1; i<4; i++)
	{
		for(int j =1; j<4; j++)
		{
			if(check_press(i,j))
			{
				return (i*3+j+1);
			}
		}
	}
	if(check_press(3,1))
	{
		return 0;
	}
	
	return -1;
}
struct
{
	unsigned int sec: 6;
	unsigned int min: 6;
	unsigned int hour: 5;
	unsigned int day: 9;
	unsigned int month: 4;
	unsigned int year: 14;
}calendar;
bool isLeapYear(unsigned int year)
{
	if(year%4 != 0)
	{
		return false;
	}
	else if(year%100 !=0)
	{
		return true;
	}
	else if(year%400 == 0)
	{
		return true;
	}
	else
	return false;
}
bool check_press(int c, int r)
{
	
	for(int i = 4; i<8;i++)
	{
		SET_BIT(DDRC,i);
		CLR_BIT(PORTC,i);
	}
	CLR_BIT(DDRC,c);
	SET_BIT(PORTC,c);
	volatile int j = 0;
	for(j; j<1000;j++)
	{
		//wait some cycles for set bit and mode to change
	}
	bool col_on = GET_BIT(PINC,c)== 0;
	for(int i = 0; i<4;i++)
	{
		SET_BIT(DDRC,i);
		CLR_BIT(PORTC,i);
	}
	CLR_BIT(DDRC,r+4);
	SET_BIT(PORTC,r+4);
	volatile int k = 0;
	for(k; k<1000;k++)
	{
		//wait some cycles for set bit and mode to change
	}
	bool row_on = (GET_BIT(PINC,r+4) == 0);
	return( col_on && row_on);
}

int main (void)
{
	board_init();
	ini_lcd();
	ini_avr();
	
	calendar.sec =50 ;
	calendar.min = 59;
	calendar.hour= 23;
	calendar.day=28;
	calendar.month=2;
	calendar.year=5;
	//watchdog registers
	/*if(MCUCSR & _BV(WDRF))
	{
		MCUCSR &= ~_BV(WDRF);
		WDTCSR |= (_BV(WDCE) | _BV(WDE));
		WDTCSR = 0x00;
		
	}*/
	//MCUCSR = 1<<JTD;
	//MCUCSR = 1<<JTD;
	bool isMilitary = true;
	bool buttonPreSta[16];
	int setState =1;
	const unsigned char dayTbl[13]={
		0,
		31,
		28,
		31,
		30,
		31,
		30,
		31,
		31,
		30,
		31,
		30,
		31
	};
	
	clr_lcd();
	//put_lcd('5');
	put_str_lcd("Cheese are you h");
	pos_lcd(1,0);
	put_str_lcd("appy now");
	char timebuff[16];
	char datebuff[16];
	int currentSetValue = 0;
	int numButtonPress = 0;
	//clr_lcd();
	for(int i = 4; i<8;i++)
	{
	SET_BIT(DDRC,i);
	CLR_BIT(PORTC,i);
	}
	CLR_BIT(DDRC,0);
	SET_BIT(PORTC,0);
	while(true)
	{
		wait_avr(1000);
		clr_lcd();
		if(check_press(0,0))
		{
			
			put_str_lcd("Hello");
		}
		
		/*if(setState >0)
		{
			
			int num = get_numberPres();
			
			switch(setState)
			{
				case 1:
				sprintf(datebuff,"--/--/----      ");
				sprintf(timebuff,"%02i:--:--        ",currentSetValue);
				if (num >=0)
				{
					
					numButtonPress++;
					currentSetValue= currentSetValue*10+num;
					
					if(numButtonPress ==2)
					{
						calendar.hour = currentSetValue;
						currentSetValue = 0;
						setState++;
					}
				}
				
				break;
				case 2:
				if (num >=0)
				{
					
					numButtonPress++;
					currentSetValue= currentSetValue*10+num;
					if(numButtonPress ==2)
					{
						calendar.min = currentSetValue;
						currentSetValue = 0;
						setState++;
					}
				}
				break;
				case 3:
				if (num >=0)
				{
					
					numButtonPress++;
					currentSetValue= currentSetValue*10+num;
					if(numButtonPress ==2)
					{
						calendar.sec = currentSetValue;
						currentSetValue = 0;
						if(isMilitary)
						{
							setState= setState+2;
						}
						else
						{
							setState++;
						}
						
					}
				}
				break;
				case 4:
				if (num >=0)
				{
					//we are checking for am pm
					
				}
				break;
				case 5:
				if (num >=0)
				{
					
					numButtonPress++;
					currentSetValue= currentSetValue*10+num;
					if(numButtonPress ==2)
					{
						calendar.month = currentSetValue;
						currentSetValue = 0;
						setState++;
					}
				}
				break;
				case 6:
				if (num >=0)
				{
					
					numButtonPress++;
					currentSetValue= currentSetValue*10+num;
					if(numButtonPress ==2)
					{
						calendar.day = currentSetValue;
						currentSetValue = 0;
						setState++;
					}
				}
				break;
				case 7:
				if (num >=0)
				{
					
					numButtonPress++;
					currentSetValue= currentSetValue*10+num;
					if(numButtonPress ==4)
					{
						calendar.year = currentSetValue;
						currentSetValue = 0;
						setState=0;
					}
				}
				break;
			}
			pos_lcd(0,0);
			put_str_lcd(datebuff);
			pos_lcd(1,0);
			put_str_lcd(timebuff);
		}*/
		//else{
			/*wait_avr(1000);
			clr_lcd();
			
			calendar.sec++;
			if(calendar.sec == 60)
			{
				calendar.min++;
				calendar.sec = 0;
			}
			if(calendar.min == 60)
			{
				
				calendar.hour++;
				calendar.min = 0;
			}
			if(calendar.hour == 24)
			{
				calendar.day++;
				calendar.hour=0;
			}
			if(calendar.day > dayTbl[calendar.month]+(isLeapYear(calendar.year) &&calendar.month ==2 ? 1 : 0))
			{
				calendar.day =1;
				calendar.month++;
			}
			if(calendar.month == 13)
			{
				calendar.year++;
				calendar.month=1;
			}
			
			if(isMilitary){
				
				sprintf(timebuff,"%02i:%02i:%02i      ",calendar.hour,calendar.min,calendar.sec);
			}
			else
			{
				int hour = calendar.hour;
				if(hour >= 12)
				{
					
					hour=hour-12;
				}
				if(hour == 0)
				{
					hour = 12;
				}
				if(calendar.hour > 11)
				{
					sprintf(timebuff,"%02i:%02i:%02ipm      ",hour, calendar.min,calendar.sec);
				}
				else{
				sprintf(timebuff,"%02i:%02i:%02iam      ",hour, calendar.min,calendar.sec);}
			}
			sprintf(datebuff,"%02i/%02i/%02i      ",calendar.month,calendar.day,calendar.year);
			pos_lcd(0,0);
			put_str_lcd(datebuff);
			pos_lcd(1,0);
			put_str_lcd(timebuff);
			if(check_press(3,2) && !buttonPreSta[3*4+2])
			{
				
				isMilitary =!isMilitary;
			}
			for(int i =0; i <4; i++)
			{
				for(int j = 0; j<4; j++)
				{
					buttonPreSta[i*4+j] = check_press(i,j);
				}
			}*/
			
		//}
		
		
	}
	// Insert application code here, after the board has been initialized.
}
