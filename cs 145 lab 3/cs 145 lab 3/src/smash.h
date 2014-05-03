/*
 * smash.h
 *
 * Created: 5/2/2014 3:47:42 PM
 *  Author: yuhao
 */ 


#ifndef SMASH_H_
#define SMASH_H_

#define SMASH_SONG_LENGTH 188

char smashStr[] = "9g09i09k09l07n09s09s09s03s07p08n08l03k17k07!08i08k08l08n03k17k08!08b08i08k08l08n"
"03k17k08f08g07i08g08f07d03k07!07p08n08l03k17k07!08i08k08l08n03k17k07!08i08k08l08"
"n03k17k08f08g07i08g08f03d07w07u05s05r07r07s07u07s05r07r07p02p05f07g07i07i07g05f0"
"7g07i07s07u03w07i07i07k07l07l07k05i07w07w07x07z07z07x05w05m07n07p07p07n07n07m05r"
"03r05k01p04w07u07w07u05s01p05p07r07s07r07p07n07l05k07k07l07k07i05g05n07n07p05n07"
"l07k05l07i07k05l07k07i07i07g03k05!05k07k07l07k07i05g05n07n07p05n07l07k05l07i07k0"
"5l07k07i01g05j02g04j07l07o07m07l07j05m03j05o02n05o02n05o07n04!03h08j07!097097097"
"0870";

Note_t smash(int ind) {
	Note_t n;
	n.frequency = getFrequency(smashStr[3 * ind + 1]);
	n.duration = getDuration(smashStr[3 * ind], n.frequency);
	n.breakAfter = getTie(smashStr[3 * ind + 2]);
	return n;
}



#endif /* SMASH_H_ */