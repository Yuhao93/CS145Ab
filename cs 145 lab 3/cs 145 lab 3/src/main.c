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

#include "notes.h"
#include "still_alive.h"

int get_numberPres(bool buttonPreSta[]);
bool check_press(int r, int c);

void playSong(unsigned int song, unsigned int noteCount);
void playNote(Note_t note);

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

void playSong(unsigned int song, unsigned int noteCount) {
	// Small rest in between playing notes
	Note_t n;
	n.frequency = getFrequency('!');
	n.duration = getDuration('8', n.frequency);
	n.breakAfter = getTie('0');
	
	for (unsigned int i = 0; i < noteCount; i++) {
		Note_t note;
		switch (song) {
			case 0:
				note = stillAlive(i);
				break;
		}
		
		
		playNote(note);
		// Only play rest in between notes if we are not tieing into
		// the next note
		if (note.breakAfter) { playNote(n); }
	}
}

void playNote(Note_t note) {
	for (unsigned int i = 0; i < note.duration; i++) {
		if (note.frequency != REST) { SET_BIT(PORTB, 0); }
		wait_avr(note.frequency);
		if (note.frequency != REST) { CLR_BIT(PORTB, 0); }
		wait_avr(note.frequency);
	}
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
	SET_BIT(DDRB, 0);
	
	unsigned int play = 0;
	while(true)
	{
		if (check_press(0, 0)) {
			play = 1;
		}
		if (play == 1) {
			clr_lcd();
			put_str_lcd("Now playing...");
			pos_lcd(1, 0);
			put_str_lcd("Still Alive");
			playSong(0, STILL_ALIVE_SONG_LENGTH);
		}
		play = 0;
	}
}
