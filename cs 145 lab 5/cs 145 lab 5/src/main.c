#include <asf.h>
#include <avr.h>
#include <lcd.h>
#include <stdio.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "notes.h"
#include "sdCard.h"

const char PROGMEM stillAlive[] PROGMEM =
"7j07i07g06g06i01!07!07907j07i07g05g05i07!05e07g05902!07905g07i04j07g05d04e05g079"
"05905i01!07!07j07i07g06g06i01!07!07907j07i07g07g05!07i07e05!07g07901!05g07i04j07"
"g04d07e07g07!07907e07g07h07g07e07c05!07907a05c05h07g07e07e07c07e07c05c07c07!0790"
"7a05c05h07j07h07g07e07e07g05h07h07!07j07l07m07m05l05j07h07j07l07l05j05h07e07c07e"
"07h07h05g07g07i04i07!07e05b07!07b05e07!07e05b07!07b05e07!07e05b07!07b05e07!07907"
"j07i07g06g06i01!05!07j07i07g04g07i05e05g05902!07!05g07i04j05g05d07e04g07905905i0"
"1!07907j07i07g06g06i01!07!07907j07i07g07g05!07i07e05!07g07907902!07!05g07i04j07g"
"04d07e07g07!07907e07g07h07g07e07c05!07907a05c05h07g07e07e07c07e07c05c05c07907a05"
"c05h07j07h07g07e07e07g05h05h07j07l07m07m05l05j07h07j07l07l07j07h05h07e07c07e07h0"
"7h05g07g07i04i07!07e05b07!07b05e07!07e05b07!07b05e07!07e05b07!07b05e05!07j07i07g"
"06g06i01!07!07907j07i07g07g05!07i07e05!07g07901!05g07i04j07g04d07e05g05905905i01"
"!07!07j07i07g06g06i01!05!07j07i07g07g05!07i07e05!07g07901!05g07i04j07g04d07e05g0"
"7907e07g07h07g07e07c05!07907a05c05h07g07e07e07c07e07c05c05c07907a05c05h07j07h07g"
"07e07e07g05h05h07j07l07m07m07l07j05j07h07j07l07l07j07h05h07e07c07e07h07h05g07g07"
"i04i03!07l07l07n07l07i05e07g07i04i04!07l07l07l07n07l07i05e07g07i04i04!07l07l07l0"
"7n07l07i05e07g07i04i03!07l07l07n07l07i05e07g07i04i04!07l07l07l07n07l07i05e07g07i"
"04i04!07j07l05l03!07j07i05i0";

const char PROGMEM mario[] PROGMEM =
"7g07g07!07g07!07c07g07!07j04!07704!07c05!07705!07405!07907!07b07!07a07907!07707g"
"07j07l07!07h07j07!07g07!07c07e07b05!07c05!07705!07405!07907!07b07!07a07907!07707"
"g07j07l07!07h07j07!07g07!07c07e07b03!07j07i07h07f07!07g07!07807907c07!07907c07e0"
"5!07j07i07h07f07!07g07!07o07!07o07o03!07!07j07i07h07f07!07g07!07807907c07!07907c"
"07e05!07f05!07e05!07c01!07!07j07i07h07f07!07g07!07807907c07!07907c07e05!07j07i07"
"h07f07!07g07!07o07!07o07o03!07!07j07i07h07f07!07g07!07807907c07!07907c07e05!07f0"
"5!07e05!07c02!07!07c07c07!07c07!07c07e07!07g07c07!07907704!07c07c07!07c07!07c07e"
"07g01!07c07c07!07c07!07c07e07!07g07c07!07907704!07g07g07!07g07!07c07g07!07j04!07"
"704!07c05!07705!07405!07907!07b07!07a07907!07707g07j07l07!07h07j07!07g07!07c07e0"
"7b05!07c05!07705!07405!07907!07b07!07a07907!07707g07j07l07!07h07j07!07g07!07c07e"
"07b05!07g07c07!07705!07807!07907h07!07h07904!07b07l07l07l07j07h07g05c07903707g07"
"c07!07705!07807!07907h07!07h07904!07907h07!07h07h07g07e07c07407!07407004!07g07c0"
"7!07705!07807!07907h07!07h07904!07b07l07l07l07j07h07g05c07903707g07c07!07705!078"
"07!07907h07!07h07904!07907h07!07h07h07g07e07c07407!07407004!07c07c07!07c07!07c07"
"e07!07g07c07!07907704!07c07c07!07c07!07c07e07g01!07c07c07!07c07!07c07e07!07g07c0"
"7!07907704!07g07g07!07g07!07c07g07!07j04!07704!07g07c07!07705!07807!07907h07!07h"
"07904!07b07l07l07l07j07h07g05c07903707g07c07!07705!07807!07907h07!07h07904!07907"
"h07!07h07h07g07e07c07407!0740700";

const char PROGMEM smash[] PROGMEM = 
"9g09i09k09l07n09s09s09s03s07p08n08l03k17k07!08i08k08l08n03k17k08!08b08i08k08l08n"
"03k17k08f08g07i08g08f07d03k07!07p08n08l03k17k07!08i08k08l08n03k17k07!08i08k08l08"
"n03k17k08f08g07i08g08f03d07w07u05s05r07r07s07u07s05r07r07p02p05f07g07i07i07g05f0"
"7g07i07s07u03w07i07i07k07l07l07k05i07w07w07x07z07z07x05w05m07n07p07p07n07n07m05r"
"03r05k01p04w07u07w07u05s01p05p07r07s07r07p07n07l05k07k07l07k07i05g05n07n07p05n07"
"l07k05l07i07k05l07k07i07i07g03k05!05k07k07l07k07i05g05n07n07p05n07l07k05l07i07k0"
"5l07k07i01g05j02g04j07l07o07m07l07j05m03j05o02n05o02n05o07n04!03h08j07!097097097"
"0870";

PROGMEM const char* const string_table[] PROGMEM = {
	stillAlive,
	mario,
	smash
	};

bool check_press(int r, int c);
void playNote(Note_t note, float volume);

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

void lightLed(Note_t note) {
	if (note.frequency == REST) {
		return;
	}
	
	unsigned char ind = note.ind;
	unsigned char diff = (note.ind / 5) % 4;
	
	if (ind != 37) {
		if (ind > 17) {
			SET_BIT(PORTA, diff + 4);
		} else {
			SET_BIT(PORTB, diff);
		}
	}
}

void unlightLed(Note_t note) {
	if (note.frequency == REST) {
		return;
	}
	
	unsigned char ind = note.ind;
	unsigned char diff = (note.ind / 5) % 4;
	if (ind != 37) {
		if (ind > 17) {
			CLR_BIT(PORTA, diff + 4);
		} else {
			CLR_BIT(PORTB, diff);
		}
	}
}

void playNote(Note_t note, float volume) {
	unsigned short vFreq = (unsigned short) (note.frequency * volume);
	unsigned short reverseVFreq = (unsigned short) (note.frequency * (2 - volume));
	
	for (unsigned int i = 0; i < note.duration; i++) {
		if (note.frequency != REST) { SET_BIT(PORTA, 3); }
		wait_avr(vFreq);
		if (note.frequency != REST) { CLR_BIT(PORTA, 3); }
		wait_avr(reverseVFreq);
	}
}

unsigned int bytesToInt(unsigned char ch1, unsigned char ch2) {
	return (ch1 << 8) | ch2;
}

Song_t getSong(unsigned char* block, int offset) {
	Song_t song;
	song.title = malloc(16 * sizeof(char));
	sprintf(song.title, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", block[offset], block[offset + 1], block[offset + 2], block[offset + 3], block[offset + 4],
		block[offset + 5], block[offset + 6], block[offset + 7], block[offset + 8], block[offset + 9],
		block[offset  +10], block[offset + 11], block[offset + 12], block[offset + 13], block[offset + 14]);
	song.blockAddr = bytesToInt(block[offset + 15], block[offset + 16]);
	song.length = bytesToInt(block[offset + 17], block[offset + 18]);
	return song;
}

void updateMainScreen(Song_t* songs, int numSongs, int cursor) {
	clr_lcd();
	pos_lcd(0, 0);
	put_lcd('>');
	pos_lcd(0, 1);
	
	put_str_lcd(songs[cursor].title);
	if (numSongs > cursor + 1) {
		pos_lcd(1, 1);
		put_str_lcd(songs[cursor + 1].title);
	}
}

unsigned char* getNotes(Song_t* currentSong) {
	unsigned long numChar = currentSong->length * 3;
	unsigned char* arr = malloc(numChar);
	
	strcpy_P(arr, (char*)pgm_read_word(&(string_table[currentSong->blockAddr])));
	return arr;
}

int main (void)
{
	board_init();
	ini_lcd();
	clr_lcd();
	
	SET_BIT(DDRA, 4);
	SET_BIT(DDRA, 5);
	SET_BIT(DDRA, 6);
	SET_BIT(DDRA, 7);
	SET_BIT(DDRB, 0);
	SET_BIT(DDRB, 1);
	SET_BIT(DDRB, 2);
	SET_BIT(DDRB, 3);
	
	bool buttonPreviousState[16];
	for (int i = 0; i < 16; i++) {
		buttonPreviousState[i] = false;
	}
	
	SET_BIT(DDRA, 3);
	unsigned char block[59]  = {0, 3,
		'P', 'o', 'r', 't', 'a', 'l', '-', 'S', 't', 'i', 'l', 'l', ' ', 'A', 'l', 0, 0, 1, 180,
		'S', 'u', 'p', 'e', 'r', ' ', 'M', 'a', 'r', 'i', 'o', ' ', 'B', 'r', 'o', 0, 1, 1, 208,
		'S', 'm', 'a', 's', 'h', ' ', 'B', 'r', 'o', 's', ' ', 'B', 'r', 'a', 'w', 0, 2, 0, 188 
	};
	//sdcardRead(0, 0, block);
	
	// First two bytes store the number of songs there are
	int numSongs = bytesToInt(block[0], block[1]);


	Song_t* s1 = malloc(sizeof(Song_t));
	Song_t* s2 = malloc(sizeof(Song_t));
	// Go through each song (19 bytes, 15 for the title, 2 for the block address, 2 for the length)
	Song_t* songs = (Song_t*) malloc(sizeof(Song_t) * numSongs);
	for (int i = 0; i < numSongs; i++) {
		int offset = i * 19 + 2;
		songs[i] = getSong(block, i * 19 + 2);
	}
	
	int cursor = 0;
	
	// 0 main screen
	// 1 playing
	// 2 pause
	int state = 0;
	
	Song_t* currentSong = &songs[0];
	char* currentNotes = NULL;
	float volume = 1.0f;
	
	unsigned int progress = 0;
	
	// Small rest in between playing notes
	Note_t n;
	n.frequency = getFrequency('!');
	n.duration = getDuration('8', n.frequency);
	n.breakAfter = getTie('0');
	
	updateMainScreen(songs, numSongs, cursor);

	while(true)
	{
		switch (state) {
			case 0:
				// 8 is down
				if (check_press(1, 0) && !buttonPreviousState[1 * 4 + 0]) {
					if (cursor > 0) {
						updateMainScreen(songs, numSongs, --cursor);
					}
				// 2 is up
				} else if (check_press(1, 2) && !buttonPreviousState[1 * 4 + 2]) {
					if (cursor < numSongs - 1) {
						updateMainScreen(songs, numSongs, ++cursor);
					}
				} else if (check_press(1, 1) && !buttonPreviousState[1 * 4 + 1]) {
					clr_lcd();
					state = 1;
					progress = 0;
					currentSong = &songs[cursor];
					currentNotes = getNotes(currentSong);
				}
				break;
			
			case 1:
	
				if (progress < currentSong->length) {
					Note_t note = getNote(currentNotes, progress++);
					
					lightLed(note);
					playNote(note, volume);
					unlightLed(note);
					if (note.breakAfter) { playNote(n, volume); }
						
					
					char progressStr[] = "................";
					for (int i = 0; i < 16 * ((float) progress / currentSong->length); i++) {
						progressStr[i] = '#';
					}
					clr_lcd();
					pos_lcd(0, 1);
					put_str_lcd(currentSong->title);
					pos_lcd(1, 0);
					put_str_lcd(progressStr);
						
				} else {
					state = 0;
					progress = 0;
					currentSong = NULL;
					free(currentNotes);
					updateMainScreen(songs, numSongs, cursor);
				}
				
				
				if (check_press(1, 1) && !buttonPreviousState[1 * 4 + 1]) {
					state = 2;
					clr_lcd();
					pos_lcd(0, 1);
					put_str_lcd(currentSong->title);
					pos_lcd(1, 0);
					put_str_lcd("Paused");
				}
				
				if (check_press(3, 0)  && !buttonPreviousState[3 * 4 + 0]) {
					state = 0;
					progress = 0;
					currentSong = NULL;
					free(currentNotes);
					updateMainScreen(songs, numSongs, cursor);
				}
				
				if (check_press(1, 0) && !buttonPreviousState[1 * 4 + 0]) {
					if (volume >= .1f) {
						volume -= .1f;
					}
				}
				
				if (check_press(1, 2) && !buttonPreviousState[1 * 4 + 2]) {
					if (volume < 1.9f) {
						volume += .1f;
					}
				}
				
				if (check_press(2, 1) && !buttonPreviousState[2 * 4 + 1]) {
					if (cursor < numSongs - 1) {
						cursor ++;
						clr_lcd();
						progress = 0;
						currentSong = &songs[cursor];
						free(currentNotes);
						currentNotes = getNotes(currentSong);
					}	
				}
				if (check_press(0, 1) && !buttonPreviousState[0 * 4 + 1]) {
					if (cursor > 0) {
						cursor--;
						clr_lcd();
						progress = 0;
						currentSong = &songs[cursor];
						free(currentNotes);
						currentNotes = getNotes(currentSong);
					}
 				}
				break;
				
			case 2:
				if (check_press(1, 1) && !buttonPreviousState[1 * 4 + 1]) {
					state = 1;
					clr_lcd();
				} else if (check_press(3, 0) && !buttonPreviousState[1 * 4 + 3]) {
					state = 0;
					progress = 0;
					currentSong = NULL;
					free(currentNotes);
					updateMainScreen(songs, numSongs, cursor);
				}
				break;
		}
		
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				buttonPreviousState[i * 4 + j] = check_press(i, j);
			}
		}
	}
}
