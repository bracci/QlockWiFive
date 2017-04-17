/**
   Woerter_IT
   Definition der italienischen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

	 01234567890
   0 SONORLEBORE
   1 ERLUNASDUEZ
   2 TREOTTONOVE
   3 DIECIUNDICI
   4 DODICISETTE
   5 QUATTROCSEI
   6 CINQUESMENO
   7 ECUNOQUARTO
   8 VENTICINQUE
   9 DIECIEMEZZA

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.0
   @created  17.12.2012
*/

#ifndef WOERTER_IT_H
#define WOERTER_IT_H

/**
   Definition der Woerter.
*/
#define IT_SONOLE    matrix[0] |= 0b1111011000000000
#define IT_LE        matrix[0] |= 0b0000011000000000
#define IT_ORE       matrix[0] |= 0b0000000011100000
#define IT_E         matrix[1] |= 0b1000000000000000
#define IT_AM        matrix[0] |= 0b1000000000000000
#define IT_PM        matrix[0] |= 0b0100000000000000

#define IT_H_LUNA    matrix[1] |= 0b0011110000000000
#define IT_H_DUE     matrix[1] |= 0b0000000111000000
#define IT_H_TRE     matrix[2] |= 0b1110000000000000
#define IT_H_OTTO    matrix[2] |= 0b0001111000000000
#define IT_H_NOVE    matrix[2] |= 0b0000000111100000
#define IT_H_DIECI   matrix[3] |= 0b1111100000000000
#define IT_H_UNDICI  matrix[3] |= 0b0000011111100000
#define IT_H_DODICI  matrix[4] |= 0b1111110000000000
#define IT_H_SETTE   matrix[4] |= 0b0000001111100000
#define IT_H_QUATTRO matrix[5] |= 0b1111111000000000
#define IT_H_SEI     matrix[5] |= 0b0000000011100000
#define IT_H_CINQUE  matrix[6] |= 0b1111110000000000

#define IT_MENO      matrix[6] |= 0b0000000111100000
#define IT_E2        matrix[7] |= 0b1000000000000000
#define IT_UN        matrix[7] |= 0b0011000000000000
#define IT_QUARTO    matrix[7] |= 0b0000011111100000
#define IT_VENTI     matrix[8] |= 0b1111100000000000
#define IT_CINQUE    matrix[8] |= 0b0000011111100000
#define IT_DIECI     matrix[9] |= 0b1111100000000000
#define IT_MEZZA     matrix[9] |= 0b0000001111100000

#endif
