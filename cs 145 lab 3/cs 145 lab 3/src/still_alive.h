/*
 * still_alive.h
 *
 * Created: 4/28/2014 11:35:28 PM
 *  Author: yuhao
 */ 


#ifndef STILL_ALIVE_H_
#define STILL_ALIVE_H_

#include "notes.h"

// how many notes there are in the song
#define STILL_ALIVE_SONG_LENGTH 436

// A "compiled" song. Every 3 letters corresponds to a note
// letter 1 corresponds to a duration
// letter 2 corresponds to a frequence
// letter 3 corresponds to whether the note ties into the next one
char str[] = 
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

// Get the nth note from the song
Note_t stillAlive(int ind) {
	Note_t n;
	n.frequency = getFrequency(str[3 * ind + 1]);
	n.duration = getDuration(str[3 * ind], n.frequency);
	n.breakAfter = getTie(str[3 * ind + 2]);
	return n;
};

#endif /* STILL_ALIVE_H_ */