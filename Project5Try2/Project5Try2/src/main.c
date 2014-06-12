#include <asf.h>
#include <inttypes.h>
#include "i2cmaster.h"
#include "avr.h"
#include <stdio.h>
#include "adxl.h"
#include <stdlib.h>

#define DISP(ch, p, ind) (ch == 1 ? SET_BIT(p, ind) : CLR_BIT(p, ind))

void drawPattern(unsigned char* pattern) {
	// Initially disable both memory latches
	CLR_BIT(PORTA, 3);
	CLR_BIT(PORTA, 4);
	
	// For each layer
	for (int i = 0; i < 4; i++) {
		// Depending on the layer, toggle the selecting bits
		switch (i) {
			case 0:
				CLR_BIT(PORTB, 1);
				CLR_BIT(PORTB, 0);
				break;
			
			case 1:
				SET_BIT(PORTB, 1);
				CLR_BIT(PORTB, 0);
				break;
			
			case 2:
				CLR_BIT(PORTB, 1);
				SET_BIT(PORTB, 0);
				break;
			
			case 3:
				SET_BIT(PORTB, 1);
				SET_BIT(PORTB, 0);
				break;
		}
		
		// Display half of the pattern for one layer
		DISP(pattern[i * 16], PORTC, 2);
		DISP(pattern[i * 16 + 1], PORTC, 3);
		DISP(pattern[i * 16 + 2], PORTC, 4);
		DISP(pattern[i * 16 + 3], PORTC, 5);
		DISP(pattern[i * 16 + 4], PORTC, 6);
		DISP(pattern[i * 16 + 5], PORTC, 7);
		DISP(pattern[i * 16 + 6], PORTA, 6);
		DISP(pattern[i * 16 + 7], PORTA, 5);
		
		// Quickly toggle the write enable on and off to load into one 8bit memory latch
		SET_BIT(PORTA, 3);
		CLR_BIT(PORTA, 3);
		
		// Display the other half of the pattern for one layer
		DISP(pattern[i * 16 + 8], PORTC, 2);
		DISP(pattern[i * 16 + 9], PORTC, 3);
		DISP(pattern[i * 16 + 10], PORTC, 4);
		DISP(pattern[i * 16 + 11], PORTC, 5);
		DISP(pattern[i * 16 + 12], PORTC, 6);
		DISP(pattern[i * 16 + 13], PORTC, 7);
		DISP(pattern[i * 16 + 14], PORTA, 6);
		DISP(pattern[i * 16 + 15], PORTA, 5);
		
		// Quickly toggle the write enable on and off to load into the other 8bit memory latch
		SET_BIT(PORTA, 4);
		CLR_BIT(PORTA, 4);
		
		// Sleep for 200ms
		wait_avr(200);
	}
}

// Struct used to sort for calculating which leds to  light up
typedef struct SortableInd {
	int ind;
	float score;
} t_SortableInd;

// function that compares two t_SortableInd instances
int compareSortable(const void* v1, const void* v2) {
	t_SortableInd* s1 = v1;
	t_SortableInd* s2 = v2;
	
	if (s1->score > s2->score) {
		return 1;
	} else if (s1->score < s2->score) {
		return -1;
	} else {
		return 0;
	}
}

int main (void)
{
	board_init();
	initAdxl();
	
	// Set all used ports to output
	SET_BIT(DDRC, 2);
	SET_BIT(DDRC, 3);
	SET_BIT(DDRC, 4);
	SET_BIT(DDRC, 5);
	SET_BIT(DDRC, 6);
	SET_BIT(DDRC, 7);
	
	SET_BIT(DDRA, 3);
	SET_BIT(DDRA, 4);
	SET_BIT(DDRA, 5);
	SET_BIT(DDRA, 6);
	
	SET_BIT(DDRB, 0);
	SET_BIT(DDRB, 1);
	
	// Initalize the pattern to all off, each char is a 1 or 0
	unsigned char pattern[64];
	for (int i = 0; i < 64; i++) {
		pattern[i] = 0;
	}
	
	// buffer for the axis data, each value is >= -1 and <= 1
	float axes[3];
	
	// buffer for the sortableInd objects
	t_SortableInd inds[64];
	
	while (true) {
		// Get the accelerometer data
		readAdxl(axes);
		
		// For every led
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				for (int z = 0; z < 4; z++) {
					// Get its index
					int ind = y * 16 + x * 4 + z;
					
					// Initially set it to 0
					pattern[ind] = 0;
					
					// For the corresponding struct, set its index
					inds[ind].ind = ind;
					
					// Give it a "score". The accelerometer value returned is a vector
					// that points in the normal of gravity. We can think of the led as
					// a 3d point, or another vector. The leds we want to light up are the
					// ones that are pointing the "most down", meaning the ones with the smallest
					// (largest negative) component along the gravity vector.
					// To get this component, we simply take the dot product of the point and the
					// gravity vector. This acts as a score. Those with the smallest score, i.e.
					// those that are pointing down the most, are picked. 
					inds[ind].score = -y * axes[0] + z * axes[1] + -x * axes[2];
				}
			}
		}
		
		// Use quicksort to sort the structs based on the score
		qsort(inds, 64, sizeof(t_SortableInd), compareSortable);
		
		// Pick the 32 structs with the smallest score and enable those leds
		for (int i = 0; i < 32; i++) {
			pattern[inds[i].ind] = 1;
		}
		
		// Draw the pattern
		drawPattern(pattern);
	}
}
