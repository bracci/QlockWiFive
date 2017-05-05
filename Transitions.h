/**
 * Transitions.h
 * Transitionen bei Wechsel Zeitmatrix
 *
 * @mc       Arduino/UNO
 * @autor    Manuel Bracher / manuel.bracher@gmail.com
 * @version  1.0
 * @created  19.05.15
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */

#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "Arduino.h"

class Transitions {
  public:
    static boolean nextSlideStep (word matrixNew[16], word matrix[16]);
    static void resetTransition();
    static boolean nextMatrixStep(word matrixOld[16], word matrixNew[16], word matrixOverlay[16], word matrix[16]);

  private:
    static void shiftColumnDown(word matrix[16], byte column);
    static void shiftColumnUp (word matrixNew[16], word matrix[16], byte column);
    static void shiftDownMatrixErease(word matrixMatrix[16], word matrixWeak[16]);
    static void shiftDownMatrixWrite(word matrixMatrix[16], word matrixWeak[16]);
    static byte _counter;
    static word _usedColumns;
    static byte _remainingCoulumnCount;
    static boolean _ereasingDone;
    static boolean _writingDone;
};


#endif
