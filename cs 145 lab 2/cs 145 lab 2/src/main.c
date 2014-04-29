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
		return currValue > 12  || currValue < 1 ? 1 : currValue;
		case 6:
		return currValue > (isLeapYear(cal->year) && cal->month == 2 ? dayTbl[cal->month] + 1 : dayTbl[cal->month])  || currValue < 1? 1 : currValue;
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
	bool buttonPreSta[16];
	for (int i = 0; i < 16; i++)
	{
		buttonPreSta[i] = false;
	}
	int setState = 1;
	
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
	char timebuff[17];
	char datebuff[17];
	int currentSetValue = 0;
	int numButtonPress = 0;
	bool isMilitary = true;
	
	printData(currentSetValue, setState, &calendar, datebuff, timebuff, isMilitary);
	while(true)
	{
		if(setState > 0)
		{
			int num = get_numberPres(buttonPreSta);
			bool skip = (check_press(3, 2) && !buttonPreSta[3 * 4 + 2]);
			if (num >= 0 || skip) {
				if (num >= 0) {
				numButtonPress++;
				currentSetValue = currentSetValue * 10 + num;
				}
				if (setState ==1) {
					if (numButtonPress == 2 || skip)
					{
						calendar.hour = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==2) {
					if(numButtonPress == 2 || skip)
					{
						calendar.min = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;

					}
				}
				
				else if (setState ==3) {
					if(numButtonPress == 2 || skip)
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
					if (numButtonPress == 1 || skip) {
						// If 0, then am, leave alone. If 1, then pm, add 12 hours to the "actual" time.
						calendar.hour += formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary) * 12;
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==5) {
					if(numButtonPress == 2 || skip)
					{
						calendar.month = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==6) {
					if(numButtonPress == 2 || skip)
					{
						calendar.day = currentSetValue;
						currentSetValue = 0;
						setState++;
						numButtonPress = 0;
					}
				}
				
				else if (setState ==7) {
					if(numButtonPress == 4 || skip)
					{
						calendar.year = formatValid(currentSetValue, setState, &calendar, dayTbl, isMilitary);
						calendar.day = formatValid(calendar.day, setState - 1, &calendar, dayTbl, isMilitary);
						currentSetValue = 0;
						setState = 0;
						numButtonPress = 0;
					}
				}
				
				printData(currentSetValue, setState, &calendar, datebuff, timebuff, isMilitary);
			}
		}
		else{
			
			wait_avr(1000);
			
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
			if(calendar.day > dayTbl[calendar.month]+(isLeapYear(calendar.year) &&calendar.month == 2 ? 1 : 0))
			{
				calendar.day =1;
				calendar.month++;
			}
			if(calendar.month == 13)
			{
				calendar.year++;
				calendar.month=1;
			}

			if (check_press(1, 3) && !buttonPreSta[1 * 4 + 3]) {
				isMilitary =!isMilitary;
			}
			
			if (check_press(0, 3) && !buttonPreSta[0 * 4 + 3]) {
				setState = 1;
				currentSetValue = 0;
				numButtonPress = 0;
			}
			printData(currentSetValue, setState, &calendar, datebuff, timebuff, isMilitary);
		}
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j< 4; j++) {
				buttonPreSta[i*4+j] = check_press(i,j);
			}
		}
	}
}
