/**
   Woerter_FR
   Definition der franzoesischen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

	 01234567890
   0 ILNESTODEUX
   1 QUATRETROIS
   2 NEUFUNESEPT
   3 HUITSIXCINQ
   4 MIDIXMINUIT
   5 ONZERHEURES
   6 MOINSOLEDIX
   7 ETRQUARTPMD
   8 VINGT-CINQU
   9 ETSDEMIEPAM

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.0
   @created  12.12.2012
*/

#ifndef WOERTER_FR_H
#define WOERTER_FR_H

/**
   Definition der Woerter
*/
// Trait = Bindestrich (-)
#define FR_TRAIT        matrix[8] |= 0b0000010000000000
#define FR_ET           matrix[7] |= 0b1100000000000000
#define FR_LE           matrix[6] |= 0b0000001100000000
#define FR_MOINS        matrix[6] |= 0b1111100000000000
#define FR_ILEST        matrix[0] |= 0b1101110000000000
#define FR_HEURE        matrix[5] |= 0b0000011111000000
#define FR_HEURES       matrix[5] |= 0b0000011111100000
#define FR_AM           matrix[7] |= 0b0000000001100000
#define FR_PM           matrix[9] |= 0b0000000011000000

#define FR_CINQ         matrix[8] |= 0b0000001111000000
#define FR_DIX          matrix[6] |= 0b0000000011100000
#define FR_QUART        matrix[7] |= 0b0001111100000000
#define FR_VINGT        matrix[8] |= 0b1111100000000000
#define FR_DEMIE        matrix[9] |= 0b0001111100000000

#define FR_H_UNE        matrix[2] |= 0b0000111000000000
#define FR_H_DEUX       matrix[0] |= 0b0000000111100000
#define FR_H_TROIS      matrix[1] |= 0b0000001111100000
#define FR_H_QUATRE     matrix[1] |= 0b1111110000000000
#define FR_H_CINQ       matrix[3] |= 0b0000000111100000
#define FR_H_SIX        matrix[3] |= 0b0000111000000000
#define FR_H_SEPT       matrix[2] |= 0b0000000111100000
#define FR_H_HUIT       matrix[3] |= 0b1111000000000000
#define FR_H_NEUF       matrix[2] |= 0b1111000000000000
#define FR_H_DIX        matrix[4] |= 0b0011100000000000
#define FR_H_ONZE       matrix[5] |= 0b1111000000000000
#define FR_H_MIDI       matrix[4] |= 0b1111000000000000
#define FR_H_MINUIT     matrix[4] |= 0b0000011111100000

#endif
