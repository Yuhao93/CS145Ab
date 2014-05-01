/*
* notes.h
*
* Created: 4/28/2014 6:14:51 PM
*  Author: yuhao
*/


#ifndef NOTES_H_
#define NOTES_H_

#include "lcd.h"

#define C5 120
#define CSHARP5 113
#define D5 106
#define DSHARP5 100
#define E5 95
#define F5 90
#define FSHARP5 85
#define G5 80
#define GSHARP5 75
#define A5 71
#define ASHARP5 68
#define B5 64

#define C6 60
#define CSHARP6 56
#define D6 53
#define DSHARP6 50
#define E6 47
#define F6 45
#define FSHARP6 42
#define G6 40
#define GSHARP6 38
#define A6 36
#define ASHARP6 34
#define B6 32

#define C7 30
#define CSHARP7 28
#define D7 27
#define DSHARP7 25
#define E7 24
#define F7 22
#define FSHARP7 21
#define G7 20
#define GSHARP7 19
#define A7 18
#define ASHARP7 17
#define B7 16

#define REST 1

#define FACTOR 2000

typedef struct Note {
	unsigned long frequency;
	unsigned long duration;
	bool breakAfter;
} Note_t;
unsigned char val = 0;

unsigned long getFrequency(char frequency) {
	if (frequency == '0') {
		return C5;
	} else
	if (frequency == '1') {
		return CSHARP5;
	} else
	if (frequency == '2') {
		return D5;
	} else
	if (frequency == '3') {
		return DSHARP5;
	} else
	if (frequency == '4') {
		return E5;
	} else
	if (frequency == '5') {
		return F5;
	} else
	if (frequency == '6') {
		return FSHARP5;
	} else
	if (frequency == '7') {
		return G5;
	} else
	if (frequency == '8') {
		return GSHARP5;
	} else
	if (frequency == '9') {
		return A5;
	} else
	if (frequency == 'a') {
		return ASHARP5;
	} else
	if (frequency == 'b') {
		return B5;
	} else
	if (frequency == 'c') {
		return C6;
	} else
	if (frequency == 'd') {
		return CSHARP6;
	} else
	if (frequency == 'e') {
		return D6;
	} else
	if (frequency == 'f') {
		return DSHARP6;
	} else
	if (frequency == 'g') {
		return E6;
	} else
	if (frequency == 'h') {
		return F6;
	} else
	if (frequency == 'i') {
		return FSHARP6;
	} else
	if (frequency == 'j') {
		return G6;
	} else
	if (frequency == 'k') {
		return GSHARP6;
	} else
	if (frequency == 'l') {
		return A6;
	} else
	if (frequency == 'm') {
		return ASHARP6;
	} else
	if (frequency == 'n') {
		return B6;
	} else
	if (frequency == 'o') {
		return C7;
	} else
	if (frequency == 'p') {
		return CSHARP7;
	} else
	if (frequency == 'q') {
		return D7;
	} else
	if (frequency == 'r') {
		return DSHARP7;
	} else
	if (frequency == 's') {
		return E7;
	} else
	if (frequency == 't') {
		return F7;
	} else
	if (frequency == 'u') {
		return FSHARP7;
	} else
	if (frequency == 'v') {
		return G7;
	} else
	if (frequency == 'w') {
		return GSHARP7;
	} else
	if (frequency == 'x') {
		return A7;
	} else
	if (frequency == 'y') {
		return ASHARP7;
	} else
	if (frequency == 'z') {
		return B7;
	} else
	if (frequency == '!') {
		return REST;
	}
	return REST;
}

unsigned long getDuration(char duration, unsigned long frequency) {
	if (duration == '0') {
		return 24 * FACTOR / frequency;
	} else
	if (duration == '1') {
		return 16 * FACTOR / frequency;
	} else
	if (duration == '2') {
		return 12 * FACTOR / frequency;
	} else
	if (duration == '3') {
		return 8 * FACTOR / frequency;
	} else
	if (duration == '4') {
		return 6 * FACTOR / frequency;
	} else
	if (duration == '5') {
		return 4 * FACTOR / frequency;
	} else
	if (duration == '6') {
		return 3 * FACTOR / frequency;
	} else
	if (duration == '7') {
		return 2 * FACTOR / frequency;
	} else
	if (duration == '8') {
		return 1 * FACTOR / frequency;
	}
	return 0;
}

bool getTie(char tie) {
	if (tie == '0') {
		return true;
	} else {
		return false;
	}
}

#endif /* NOTES_H_ */