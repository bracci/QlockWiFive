/*
   Woerter_DE
   Definition der deutschen Woerter fuer die Zeitansage.
   Die Woerter sind Bitmasken fuer die Matrix.

	 01234567890
   0 ESKISTAFUNF
   1 ZEHNZWANZIG
   2 DREIVIERTEL
   3 VORFUNKNACH
   4 HALBAELFUNF
   5 EINSXAMZWEI
   6 DREIPMJVIER
   7 SECHSNLACHT
   8 SIEBENZWOLF
   9 ZEHNEUNKUHR

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.0
   @created  18.3.2012
*/

#ifndef WOERTER_DE_H
#define WOERTER_DE_H

/**
   Definition der Woerter
*/
#define DE_VOR          matrix[3] |= 0b1110000000000000
#define DE_NACH         matrix[3] |= 0b0000000111100000
#define DE_ESIST        matrix[0] |= 0b1101110000000000
#define DE_UHR          matrix[9] |= 0b0000000011100000
#define DE_AM           matrix[5] |= 0b0000011000000000
#define DE_PM           matrix[6] |= 0b0000110000000000

#define DE_FUENF        matrix[0] |= 0b0000000111100000
#define DE_ZEHN         matrix[1] |= 0b1111000000000000
#define DE_VIERTEL      matrix[2] |= 0b0000111111100000
#define DE_ZWANZIG      matrix[1] |= 0b0000111111100000
#define DE_HALB         matrix[4] |= 0b1111000000000000
#define DE_DREIVIERTEL  matrix[2] |= 0b1111111111100000

#define DE_H_EIN        matrix[5] |= 0b1110000000000000
#define DE_H_EINS       matrix[5] |= 0b1111000000000000
#define DE_H_ZWEI       matrix[5] |= 0b0000000111100000
#define DE_H_DREI       matrix[6] |= 0b1111000000000000
#define DE_H_VIER       matrix[6] |= 0b0000000111100000
#define DE_H_FUENF      matrix[4] |= 0b0000000111100000
#define DE_H_SECHS      matrix[7] |= 0b1111100000000000
#define DE_H_SIEBEN     matrix[8] |= 0b1111110000000000
#define DE_H_ACHT       matrix[7] |= 0b0000000111100000
#define DE_H_NEUN       matrix[9] |= 0b0001111000000000
#define DE_H_ZEHN       matrix[9] |= 0b1111000000000000
#define DE_H_ELF        matrix[4] |= 0b0000011100000000
#define DE_H_ZWOELF     matrix[8] |= 0b0000001111100000

#endif
