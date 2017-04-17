/**
   Woerter_NL
   Definition der niederlaendischen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

   @mc       Arduino/RBBB
   @autor    Rudolf Klimesch (Vorlage: Christian Aschoff)
   @version  1.01
   @created  17.1.2013
   @update   29.9.2014

   Historie:
   V 1.01 - Falsches O bei ZEVEN behoben.

*/

#ifndef WOERTER_NL_H
#define WOERTER_NL_H

/*
   VORLAGE FUER DIE MATRIX

	H E T K I S A V I J F     HET=ES, IS=IST, VIJF=FUNF
	T I E N B T Z V O O R     TIEN=ZEHN, VOOR=VOR
	O V E R M E K W A R T     OVER=NACH, KWART=VIERTEL
	H A L F S P W O V E R     HALF=HALB, OVER=NACH
	V O O R T H G E E N S     VOOR=VOR, EENS=EINS
	T W E E P V C D R I E     TWEE=ZWEI, DRIE=DREI
	V I E R V I J F Z E S     VIER=VIER, VIJF=FUNF, ZES=SECHS
	Z E V E N O N E G E N     ZEVEN=SIEBEN, NEGEN=NEUN
	A C H T T I E N E L F     ACHT=ACHT, TIEN=ZEHN, ELF=ELF
	T W A A L F B F U U R     TWAALF=ZWOLF, UUR=STUNDE
*/

/**
   Definition der Woerter
*/
#define NL_VOOR         matrix[1] |= 0b0000000111100000 // VOR
#define NL_OVER         matrix[2] |= 0b1111000000000000 // NACH
#define NL_VOOR2        matrix[4] |= 0b1111000000000000 // VOR2
#define NL_OVER2        matrix[3] |= 0b0000000111100000 // NACH2
#define NL_HETIS        matrix[0] |= 0b1110110000000000 // ESIST
#define NL_UUR          matrix[9] |= 0b0000000011100000 // UHR
#define NL_AM           matrix[0] |= 0b1000000000000000
#define NL_PM           matrix[0] |= 0b0100000000000000

#define NL_VIJF         matrix[0] |= 0b0000000111100000 // FUENF
#define NL_TIEN         matrix[1] |= 0b1111000000000000 // ZEHN
#define NL_KWART        matrix[2] |= 0b0000001111100000 // VIERTEL
#define NL_ZWANZIG      matrix[1] |= 0b0000111111100000 // ZWANZIG
#define NL_HALF         matrix[3] |= 0b1111000000000000 // HALB

#define NL_H_EEN        matrix[4] |= 0b0000000111000000 // H_EIN
#define NL_H_EENS       matrix[4] |= 0b0000000111100000 // H_EINS
#define NL_H_TWEE       matrix[5] |= 0b1111000000000000 // H_ZWEI
#define NL_H_DRIE       matrix[5] |= 0b0000000111100000 // H_DREI
#define NL_H_VIER       matrix[6] |= 0b1111000000000000 // H_VIER
#define NL_H_VIJF       matrix[6] |= 0b0000111100000000 // H_FUENF
#define NL_H_ZES        matrix[6] |= 0b0000000011100000 // H_SECHS
#define NL_H_ZEVEN      matrix[7] |= 0b1111100000000000 // H_SIEBEN
#define NL_H_ACHT       matrix[8] |= 0b1111000000000000 // H_ACHT
#define NL_H_NEGEN      matrix[7] |= 0b0000001111100000 // H_NEUN
#define NL_H_TIEN       matrix[8] |= 0b0000111100000000 // H_ZEHN
#define NL_H_ELF        matrix[8] |= 0b0000000011100000 // H_ELF
#define NL_H_TWAALF     matrix[9] |= 0b1111110000000000 // H_ZWOELF

#endif
