/**
   Woerter_ES
   Definition der spanischen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

	 01234567890
   0 ESONELASUNA
   1 DOSITRESORE
   2 CUATROCINCO
   3 SEISASIETEN
   4 OCHONUEVEYO
   5 LADIEZSONCE
   6 DOCELYMENOS
   7 OVEINTEDIEZ
   8 VEINTICINCO
   9 MEDIACUARTO

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.0
   @created  17.12.2012
   @updated  13.5.2013
*/

#ifndef WOERTER_ES_H
#define WOERTER_ES_H

/**
   Definition der Woerter.
*/
//                            0       ESONELASUNA
#define ES_SONLAS      matrix[0] |= 0b0111011100000000
//                            0       ESONELASUNA
#define ES_ESLA        matrix[0] |= 0b1100011000000000

#define ES_AM          matrix[0] |= 0b1000000000000000
#define ES_PM          matrix[0] |= 0b0100000000000000

//                            6       DOCELYMENOS
#define ES_Y           matrix[6] |= 0b0000010000000000
//                            6       DOCELYMENOS
#define ES_MENOS       matrix[6] |= 0b0000001111100000

//                            8       VEINTICINCO
#define ES_CINCO       matrix[8] |= 0b0000001111100000
//                            7       OVEINTEDIEZ
#define ES_DIEZ        matrix[7] |= 0b0000000111100000
//                            9       MEDIACUARTO
#define ES_CUARTO      matrix[9] |= 0b0000011111100000
//                            7       OVEINTEDIEZ
#define ES_VEINTE      matrix[7] |= 0b0111111000000000
//                            8       VEINTICINCO
#define ES_VEINTICINCO matrix[8] |= 0b1111111111100000
//                            9       MEDIACUARTO
#define ES_MEDIA       matrix[9] |= 0b1111100000000000

//                            0       ESONELASUNA
#define ES_H_UNA       matrix[0] |= 0b0000000011100000
//                            1       DOSITRESORE
#define ES_H_DOS       matrix[1] |= 0b1110000000000000
//                            1       DOSITRESORE
#define ES_H_TRES      matrix[1] |= 0b0000111100000000
//                            2       CUATROCINCO
#define ES_H_CUATRO    matrix[2] |= 0b1111110000000000
//                            2       CUATROCINCO
#define ES_H_CINCO     matrix[2] |= 0b0000001111100000
//                            3       SEISASIETEN
#define ES_H_SEIS      matrix[3] |= 0b1111000000000000
//                            3       SEISASIETEN
#define ES_H_SIETE     matrix[3] |= 0b0000011111000000
//                            4       OCHONUEVEYO
#define ES_H_OCHO      matrix[4] |= 0b1111000000000000
//                            4       OCHONUEVEYO
#define ES_H_NUEVE     matrix[4] |= 0b0000111110000000
//                            5       LADIEZSONCE
#define ES_H_DIEZ      matrix[5] |= 0b0011110000000000
//                            5       LADIEZSONCE
#define ES_H_ONCE      matrix[5] |= 0b0000000111100000
//                            6       DOCELYMENOS
#define ES_H_DOCE      matrix[6] |= 0b1111000000000000

#endif
