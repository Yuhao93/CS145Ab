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
int get_numberPres(bool buttonPreSta[])
{
	for(int i =0; i<3; i++)
	{
		for(int j =0; j<3; j++)
		{
			if(check_press(i,j) && !buttonPreSta[i * 3 + j])
			{
				return (i*3+j+1);
			}
		}
	}
	if(check_press(3,1) && !buttonPreSta[i * 3 + j])
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

// Get the hour, correctly formatted based on the isMilitary setting
int formatHour(int hour, bool isMilitary) {
	if (isMilitary) {
		return hour;
	}
	int hour = cal.hour;
	if (hour >= 12) { 
		hour = hour - 12;
	}
	if (hour == 0) {
		hour = 12;
	}
	return hour;
}

// Checks whether or not the given input is valid for the given state.
// If not, then default to a valid value.
int formatValid(int value, int state, Calendar& cal, const unsigned char dayTbl[], bool isMilitary) {
	switch (state) {
		case 1:
			return isMilitary ? (value >= 24 ? 0 : value) : (value >= 12 ? 0 : value);
		case 2:
		case 3:
			return value >= 60 ? 0 : value;
		case 4:
			return value != 0 ? 1 : 0;
		case 5:
			return value > 12 ? 1 : value;
		case 6:
			return value > dayTbl[cal.month] ? 1 : value;
	}
	return value;
}

// Redraws the data on the LCD, changes depending on the state
void printData(int value, int state, Calendar& cal, char dateBuff[], char timeBuff[], bool isMilitary) {
	clr_lcd();
	if (state == 7 || state == 6 || state == 5 || state == 0) {
		sprintf(timeBuff,"%02i:%02i:%02i %s     ",
				formatHour(cal.hour, isMilitary),
				cal.min,
				cal.sec,
				isMilitary ? "  " : (hour >= 12 ? "pm" : "am"));
	} else if (state == 4) {
		sprintf(timeBuff,"%02i:%02i:%02i %s     ", formatHour(cal.hour, isMilitary), cal.min, cal.sec, value == 0 ? "am" : "pm");
	} else if (state == 3) {
		sprintf(timeBuff,"%02i:%02i:%02i %s     ", formatHour(cal.hour, isMilitary), cal.min, value, isMilitary ? "  " : "--");
	} else if (state == 2) {
		sprintf(timeBuff,"%02i:%02i:-- %s     ", formatHour(cal.hour, isMilitary), value, isMilitary ? "  " : "--");
	} else if (state == 1) {
		sprintf(timeBuff,"%02i:--:-- %s	    ", value, isMilitary ? "  " : "--");
	}
	
	if (state == 1 || state == 2 || state == 3 || state == 4) {
		sprintf(dateBuff, "--/--/----      ");
	} else if (state == 5) {
		sprintf(dateBuff, "%02i/--/----      ", value);
	} else if (state == 6) {
		sprintf(dateBuff, "%02i/%02i/----      ", cal.month, value);
	} else if (state == 7) {
		sprintf(dateBuff, "%02i/%02i/%04i      ", cal.month, cal.day, value);
	} else if (state == 0) {
		sprintf(dateBuff, "%02i/%02i/%04i      ", cal.month, cal.day, cal.year);
	}

	pos_lcd(0,0);
	put_str_lcd(datebuff);
	pos_lcd(1,0);
	put_str_lcd(timebuff);
}

bool check_press(int r, int c)
{
	for (int i = 0; i < 8; i++) {
		if (i < 4) {
			CLR_BIT(DDRC, i);
			SET_BIT(PORTC, i);
		} else {
			SET_BIT(DDRC, i);
			CLR_BIT(PORTC, i);
		}
	}

	/*SET_BIT(DDRC,7);//output
	SET_BIT(DDRC,6);//output
	SET_BIT(DDRC,5);//output
	SET_BIT(DDRC,4);//output
	CLR_BIT(DDRC,3);//input
	CLR_BIT(DDRC,2);//input
	CLR_BIT(DDRC,1);//input
	CLR_BIT(DDRC,0);//input
	SET_BIT(PORTC,3);//internal pull up
	SET_BIT(PORTC,2);//internal pull up
	SET_BIT(PORTC,1);//internal pull up
	SET_BIT(PORTC,0);//internal pull up
	CLR_BIT(PORTC,7);
	CLR_BIT(PORTC,6);
	CLR_BIT(PORTC,5);
	CLR_BIT(PORTC,4);*/
	//SET_BIT(PORTC,r);
	bool col_on = GET_BIT(PINC,c) == 0;
	
	for (int i = 0; i < 8; i++) {
		if (i < 4) {
			SET_BIT(DDRC, i);
			CLR_BIT(PORTC, i);
		} else {
			CLR_BIT(DDRC, i);
			SET_BIT(PORTC, i);
		}
	}
	
	/*
	CLR_BIT(DDRC,7);//output
	CLR_BIT(DDRC,6);//output
	CLR_BIT(DDRC,5);//output
	CLR_BIT(DDRC,4);//output
	SET_BIT(DDRC,3);//input
	SET_BIT(DDRC,2);//input
	SET_BIT(DDRC,1);//input
	SET_BIT(DDRC,0);//input
	CLR_BIT(PORTC,3);//internal pull up
	CLR_BIT(PORTC,2);//internal pull up
	CLR_BIT(PORTC,1);//internal pull up
	CLR_BIT(PORTC,0);//internal pull up
	SET_BIT(PORTC,7);
	SET_BIT(PORTC,6);
	SET_BIT(PORTC,5);
	SET_BIT(PORTC,4);*/
	//SET_BIT(PORTC,r);
	bool row_on = GET_BIT(PINC,r+4) == 0;
	return(row_on && col_on);
}

int main (void)
{
	board_init();
	ini_lcd();
	ini_avr();
	calendar.sec = 50 ;
	calendar.min = 59;
	calendar.hour= 23;
	calendar.day = 28;
	calendar.month = 2;
	calendar.year = 5;
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
	const unsigned char dayTbl[13] = {
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
	clr_lcd();
	while(true)
	{
		if(setState > 0)
		{
			int num = get_numberPres(buttonPreSta);
			if (num >= 0) {
				numButtonPress++;
				currentSetValue= currentSetValue * 10 + num;
				switch(setState)
				{
					case 1:
						if (numButtonPress == 2)
						{
							calendar.hour = formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary);
							currentSetValue = 0;
							setState++;
						}
					break;
				
					case 2:
						if(numButtonPress == 2)
						{
							calendar.min = formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary);
							currentSetValue = 0;
							setState++;
						}
					break;
				
					case 3:
						if(numButtonPress == 2)
						{
							calendar.sec = formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary);
							currentSetValue = 0;
							if(isMilitary)
							{
								setState += 2;
							}
							else
							{
								setState++;
							}
						}
					break;
				
					case 4:
						if (numButtonPress == 1) {
							// If 0, then am, leave alone. If 1, then pm, add 12 hours to the "actual" time.
							calendar.hour += formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary) * 12;
							currentSetValue = 0;
							setState++;
						}
					break;
				
					case 5:
						if(numButtonPress == 2)
						{
							calendar.month = formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary);
							currentSetValue = 0;
							setState++;
						}
					break;
				
					case 6:
						if(numButtonPress == 2)
						{
							calendar.day = formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary);
							currentSetValue = 0;
							setState++;
						}
					break;
				
					case 7:
						if(numButtonPress == 4)
						{
							calendar.year = formatValid(currentSetValue, setState, calendar, dayTbl, isMilitary);
							currentSetValue = 0;
							setState = 0;
						}
					break;
				}
			}
		}
		else{
			wait_avr(1000);
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
			/*
			if(isMilitary){
				
				sprintf(timebuff,"%02i:%02i:%02i			",calendar.hour,calendar.min,calendar.sec);
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
					sprintf(timebuff,"%02i:%02i:%02ipm			",hour, calendar.min,calendar.sec);
				}
				else{
				sprintf(timebuff,"%02i:%02i:%02iam			",hour, calendar.min,calendar.sec);}
			}
			sprintf(datebuff,"%02i/%02i/%02i			",calendar.month,calendar.day,calendar.year);
			pos_lcd(0,0);
			put_str_lcd(datebuff);
			pos_lcd(1,0);
			put_str_lcd(timebuff);*/
			if(check_press(3,2) && !buttonPreSta[3*4+2]) {
				isMilitary =!isMilitary;
			}
		}
		
		for(int i =0; i <4; i++) {
			for(int j = 0; j<4; j++) {
				buttonPreSta[i*4+j] = check_press(i,j);
			}
		}
		printData(currentSetValue, setState, calendar, dateBuff, timeBuff, isMilitary);
	}
}
