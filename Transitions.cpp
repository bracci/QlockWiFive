/**
 * Transitions.cpp
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

#include "Transitions.h"

byte Transitions::_counter;
word Transitions::_usedColumns;
byte Transitions::_remainingCoulumnCount;
boolean Transitions::_ereasingDone;
boolean Transitions::_writingDone;

void Transitions::resetTransition () {
  _counter = 0;
  _usedColumns = 0;
  _remainingCoulumnCount = 12;
  _ereasingDone = false;
  _writingDone = false;
}

boolean Transitions::nextSlideStep (word matrixNew[16], word matrix[16]) {
  _counter++;
  if (_counter < 22) {
    for (byte i = 0; i < min( _counter, 12); i++) {
      shiftColumnDown(matrixNew, i);
    }
  }
  else {
    for (byte i = max(_counter - 22 - 9, 0); i < min( _counter - 21, 12); i++) {
      shiftColumnUp(matrixNew, matrix, i);
    }
  }
  return (_counter > 41);
}

void Transitions::shiftColumnDown (word matrix[16], byte column) {
  word tempMatrix = (0x01 << (15 - column));
  for (byte i = 10; i > 0; i--) {
    matrix[i] &= ~tempMatrix;
    matrix[i] |= (matrix[i - 1] & tempMatrix);
  }
  matrix[0] &= ~tempMatrix;
}

void Transitions::shiftColumnUp (word matrixNew[16], word matrix[16], byte column) {
  word tempMatrix = (0x01 << (15 - column));
  byte tempIdx;
  for (byte i = 0; i < 10; i++) {
    matrixNew[i] &= ~tempMatrix;
    tempIdx = i - (9 + column - (_counter - 22));
    if (tempIdx < 12) {
      matrixNew[i] |= (matrix[tempIdx] & tempMatrix);
    }
  }
  matrix[10] &= ~tempMatrix;
}

boolean Transitions::nextMatrixStep(word matrixWeak[16], word matrixTime[16], word matrixMatrix[16], word matrix[16]) {
  byte loopCount = random(0, 5);
  byte column;

  if (!_ereasingDone) {
    shiftDownMatrixErease(matrixMatrix, matrixWeak);
    if (random(0, 3) > 0)
    {
      if ( (_remainingCoulumnCount < 3) && (_remainingCoulumnCount > 0))
      {
        matrixMatrix[0] |= ~_usedColumns;
        _usedColumns |= matrixMatrix[0];
        _remainingCoulumnCount = 0;
      }
      else
      {
        for ( byte i = 0; i < loopCount; i++) {
          column = random(4, 16);
          if (!(_usedColumns & (0x01 << column))) {
            _usedColumns |= (0x01 << column);
            _remainingCoulumnCount--;
          }
          matrixMatrix[0] |= 0x01 << column;
        }
      }
    }
  }
  else {
    shiftDownMatrixWrite(matrixMatrix, matrixWeak);
    if ( (random(0, 3) > 0) && (_remainingCoulumnCount > 0) )
    {
      if ( _remainingCoulumnCount < 3 )
      {
        matrixMatrix[0] |= ~_usedColumns;
        _usedColumns |= matrixMatrix[0];
        _remainingCoulumnCount = 0;
      }
      else
      {
        for ( byte i = 0; i < loopCount; i++) {
          column = random(4, 16);
          if (!(_usedColumns & (0x01 << column))) {
            _usedColumns |= (0x01 << column);
            _remainingCoulumnCount--;
            matrixMatrix[0] |= 0x01 << column;
          }
        }
      }
    }
  }

  if (!_remainingCoulumnCount) {
    _counter++;
    if (_counter > 11) {
      if (!_ereasingDone) {
        for (byte i = 0; i < 11; i++) {
          matrixTime[i] = matrix[i];
        }
        _usedColumns = 0;
        _remainingCoulumnCount = 12;
        _counter = 0;
        _ereasingDone = true;
      }
      else {
        _writingDone = true;
      }
    }
  }

  return (_writingDone);
}

void Transitions::shiftDownMatrixErease(word matrixMatrix[16], word matrixWeak[16]) {
  for (byte i = 10; i > 0; i--) {
    matrixMatrix[i] = matrixMatrix[i - 1];
    matrixWeak[i] |= matrixWeak[i - 1];
  }
  matrixWeak[0] = (matrixMatrix[1] | matrixWeak[1]) ;
  matrixMatrix[0] = 0;
}

void Transitions::shiftDownMatrixWrite(word matrixMatrix[16], word matrixWeak[16]) {
  for (byte i = 10; i > 0; i--) {
    matrixMatrix[i] = matrixMatrix[i - 1];
    matrixWeak[i] = matrixWeak[i - 1];
  }
  matrixWeak[0] = (~matrixMatrix[1] & matrixWeak[1]) ;
  matrixMatrix[0] = 0;
}



