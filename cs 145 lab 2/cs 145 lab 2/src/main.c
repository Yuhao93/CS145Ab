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

 
 struct Calendar
 {
	 unsigned int sec: 6;
	 unsigned int min: 6;
	 unsigned int hour: 5;
	 unsigned int day: 9;
	 unsigned int month: 4;
	 unsigned int year: 14;
 }calendar;

int get_numberPres(bool buttonPreSta[]);
bool isLeapYear(unsigned int year);
bool check_press(int r, int c);
int formatHour(int hour, bool isMilitary);
void out (char str[], int col);
int formatValid(int currValue, int state, struct Calendar* cal, const unsigned char dayTbl[], bool isMilitary);
void printData(int currValue, int state, struct Calendar* cal, char dateBuff[], char timeBuff[], bool isMilitary);


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
			if(check_press(i,j) && !buttonPreSta[i * 4 + j])
			{
				return (i*3+j+1);
			}
		}
	}
	if(check_press(3,1) && !buttonPreSta[3 * 4 + 1])
	{
		return 0;
	}
	
	return -1;
}


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

// Get the hour, correctly formatted based on the isMilitary setting
int formatHour(int hour, bool isMilitary) {
	if (isMilitary) {
		return hour;
	}
	
	if (hour >= 12) {
		hour = hour - 12;
	}
	if (hour == 0) {
		hour = 12;
	}
	return hour;
}

// Checks whether or not the given input is valid for the given state.
// If not, then default to a valid currValue.
int formatValid(int currValue, int state, struct Calendar* cal, const unsigned char dayTbl[], bool isMilitary) {
	switch (state) {
		case 1:
		return isMilitary ? (currValue >= 24 ? 0 : currValue) : (currValue >= 12 ? 0 : currValue);
		case 2:
		case 3:
		return currValue >= 60 ? 0 : currValue;
		case 4:
		return currValue != 0 ? 1 : 0;
		case 5:
		return currValue > 12 ? 1 : currValue;
		case 6:
		return currValue > dayTbl[cal->month] ? 1 : currValue;
	}
	return currValue;
}

void out (char str[], int col) {
	pos_lcd(0, col);
	put_str_lcd(str);
}

// Redraws the data on the LCD, changes depending on the state
void printData(int currValue, int state, struct Calendar* cal, char dateBuff[], char timeBuff[], bool isMilitary) {
	clr_lcd();
	if (state == 7 || state == 6 || state == 5 || state == 0) {
		sprintf(timeBuff,"%02i:%02i:%02i %s     ",
		formatHour(cal->hour, isMilitary),
		cal->min,
		cal->sec,
		isMilitary ? "  " : (cal->hour >= 12 ? "pm" : "am"));
		} else if (state == 4) {
		sprintf(timeBuff,"%02i:%02i:%02i %s     ", formatHour(cal->hour, isMilitary), cal->min, cal->sec, currValue == 0 ? "am" : "pm");
		} else if (state == 3) {
		sprintf(timeBuff,"%02i:%02i:%02i %s     ", formatHour(cal->hour, isMilitary), cal->min, currValue, isMilitary ? "  " : "--");
		} else if (state == 2) {
		sprintf(timeBuff,"%02i:%02i:-- %s     ", formatHour(cal->hour, isMilitary), currValue, isMilitary ? "  " : "--");
		} else if (state == 1) {
		sprintf(timeBuff,"%02i:--:-- %s     ", currValue, isMilitary ? "  " : "--");
	}
	
	if (state == 1 || state == 2 || state == 3 || state == 4) {
		sprintf(dateBuff, "--/--/----      ");
		} else if (state == 5) {
		sprintf(dateBuff, "%02i/--/----      ", currValue);
		} else if (state == 6) {
		sprintf(dateBuff, "%02i/%02i/----      ", cal->month, currValue);
		} else if (state == 7) {
		sprintf(dateBuff, "%02i/%02i/%04i      ", cal->month, cal->day, currValue);
		} else if (state == 0) {
		sprintf(dateBuff, "%02i/%02i/%04i      ", cal->month, cal->day, cal->year);
	}

	pos_lcd(0,0);
	put_str_lcd(dateBuff);
	pos_lcd(1,0);
	put_str_lcd(timeBuff);
}

int main (void)
{
	board_init();
	ini_lcd();
	ini_avr();
	wdt_disable();
	calendar.sec =50 ;
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
	bool buttonPreSta[16];
	for (int i = 0; i < 16; i++)
	{
		buttonPreSta[i] = false;
	}
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
	wait_avr(1000);
	clr_lcd();
	bool isMilitary = true;
	
	printData(currentSetValue, setState, &calendar, datebuff, timebuff, isMilitary);
	while(true)
	{

		/*wait_avr(1000);
		clr_lcd();
		if(check_press(0,0))
		{
		
		put_str_lcd("Hello");
		}
		
		if(setState >0)*/
		if(setState > 0)
		{
			int num = get_numberPres(buttonPreSta);
			if (num >= 0) {
				numButtonPress++;
				currentSetValue= currentSetValue * 10 + num;
				if (setState ==1) {
					if (numButtonPress == 2)
					{
						calendar.hour = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==2) {
					if(numButtonPress == 2)
					{
						calendar.min = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;

					}
				}
				
				else if (setState ==3) {
					if(numButtonPress == 2)
					{
						calendar.sec = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						if(isMilitary)
						{
							setState += 2;
						}
						else
						{
							setState++;
						}
						numButtonPress = 0;
					}
				}
				
				else if (setState ==4) {
					if (numButtonPress == 1) {
						// If 0, then am, leave alone. If 1, then pm, add 12 hours to the "actual" time.
						calendar.hour += formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary) * 12;
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==5) {
					if(numButtonPress == 2)
					{
						calendar.month = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==6) {
					if(numButtonPress == 2)
					{
						calendar.day = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==7) {
					if(numButtonPress == 4)
					{
						calendar.year = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState = 0;
						numButtonPress = 0;
					}
				}
				
				printData(currentSetValue, setState, &calendar, datebuff, timebuff, isMilitary);
			}
			/*pos_lcd(0,0);
			put_str_lcd(datebuff);
			pos_lcd(1,0);
			put_str_lcd(timebuff);
			}
			//else{
			wait_avr(1000);*/
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
			
			

			/*for(int i =0; i <4; i++)
			{
			for(int j = 0; j<4; j++)
			{
			buttonPreSta[i*4+j] = check_press(i,j);
			}
			}
			
			}*/

			//isMilitary =!isMilitary;
			if (isMilitary) {
				isMilitary = false;
			}
			else {
				isMilitary = true;
			}
			printData(currentSetValue, setState, &calendar, datebuff, timebuff, isMilitary);
		}
		for(int i =0; i <4; i++) {
			for(int j = 0; j<4; j++) {
				buttonPreSta[i*4+j] = check_press(i,j);
			}
		}
	}
}
