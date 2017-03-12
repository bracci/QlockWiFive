/**
   Woerter_EN
   Definition der englischen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

	 01234567890
   0 ITLISASTIME
   1 ACQUARTERDC
   2 TWENTYFIFEX
   3 HALFBTENFTO
   4 PASTERUNINE
   5 ONESIXTHREE
   6 FOURFIVETWO
   7 EIGHTELEVEN
   8 SEVENTWELVE
   9 TENSEOCLOCK

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.0
   @created  17.12.2012
*/

#ifndef WOERTER_EN_H
#define WOERTER_EN_H

/**
   Definition der Woerter.
*/
#define EN_ITIS     matrix[0] |= 0b1101100000000000
#define EN_TIME     matrix[0] |= 0b0000000111100000
#define EN_A        matrix[1] |= 0b1000000000000000
#define EN_OCLOCK   matrix[9] |= 0b0000011111100000
#define EN_AM       matrix[0] |= 0b0000000110000000
#define EN_PM       matrix[0] |= 0b0000000001100000

#define EN_QUATER   matrix[1] |= 0b0011111110000000
#define EN_TWENTY   matrix[2] |= 0b1111110000000000
#define EN_FIVE     matrix[2] |= 0b0000001111000000
#define EN_HALF     matrix[3] |= 0b1111000000000000
#define EN_TEN      matrix[3] |= 0b0000011100000000
#define EN_TO       matrix[3] |= 0b0000000001100000
#define EN_PAST     matrix[4] |= 0b1111000000000000

#define EN_H_NINE   matrix[4] |= 0b0000000111100000
#define EN_H_ONE    matrix[5] |= 0b1110000000000000
#define EN_H_SIX    matrix[5] |= 0b0001110000000000
#define EN_H_THREE  matrix[5] |= 0b0000001111100000
#define EN_H_FOUR   matrix[6] |= 0b1111000000000000
#define EN_H_FIVE   matrix[6] |= 0b0000111100000000
#define EN_H_TWO    matrix[6] |= 0b0000000011100000
#define EN_H_EIGHT  matrix[7] |= 0b1111100000000000
#define EN_H_ELEVEN matrix[7] |= 0b0000011111100000
#define EN_H_SEVEN  matrix[8] |= 0b1111100000000000
#define EN_H_TWELVE matrix[8] |= 0b0000011111100000
#define EN_H_TEN    matrix[9] |= 0b1110000000000000

#endif
