/**
   Woerter_D3
   Definition der schwaebischen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

	 01234567890
   0 ESKISCHFUNK
   1 DREIVIERTLA
   2 ZEHNBIEFUNF
   3 NACHGERTVOR
   4 HALBXFUNFEI
   5 OISECHSELFE
   6 ZWOIEACHTED
   7 DREIEZWOLFE
   8 ZEHNEUNEUHL
   9 SIEBNEVIERE

   @mc       Arduino/RBBB
   @autor    Fisch0204 & Chriophs007 (Unter Vorlage von Christian Aschoff)
   @version  1.0
   @created  03.03.2016
*/
#ifndef WOERTER_D3_H
#define WOERTER_D3_H

/**
   Definition der Woerter
*/
#define D3_ESISCH       matrix[0] |= 0b1101111000000000
#define D3_VOR          matrix[3] |= 0b0000000011100000
#define D3_NACH         matrix[3] |= 0b1111000000000000
#define D3_AM           matrix[0] |= 0b1000000000000000
#define D3_PM           matrix[0] |= 0b0100000000000000

#define D3_FUENF        matrix[2] |= 0b0000000111100000
#define D3_ZEHN         matrix[2] |= 0b1111000000000000
#define D3_VIERTL       matrix[1] |= 0b0000111111000000
#define D3_HALB         matrix[4] |= 0b1111000000000000
#define D3_DREIVIERTL   matrix[1] |= 0b1111111111000000

#define D3_H_OISE       matrix[5] |= 0b1111000000000000
#define D3_H_ZWOIE      matrix[6] |= 0b1111100000000000
#define D3_H_DREIE      matrix[7] |= 0b1111100000000000
#define D3_H_VIERE      matrix[9] |= 0b0000001111100000
#define D3_H_FUENFE     matrix[4] |= 0b0000011111000000
#define D3_H_SECHSE     matrix[5] |= 0b0011111100000000
#define D3_H_SIEBNE     matrix[9] |= 0b1111110000000000
#define D3_H_ACHTE      matrix[6] |= 0b0000011111000000
#define D3_H_NEUNE      matrix[8] |= 0b0001111100000000
#define D3_H_ZEHNE      matrix[8] |= 0b1111100000000000
#define D3_H_ELFE       matrix[5] |= 0b0000000111100000
#define D3_H_ZWOELFE    matrix[7] |= 0b0000011111100000

#endif
