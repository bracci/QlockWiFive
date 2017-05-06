/**
  Effects.cpp
  Klasse für diverse Effekte

  @mc       Arduino/UNO
  @autor    Manuel Bracher / manuel.bracher@gmail.com
  @version  1.0
  @created  02.01.15

  Versionshistorie:
  V 1.0:  - Erstellt.
*/

#include "Effects.h"
#include "Staben.h"

// #define DEBUG
#include "Debug.h"

void Effects::showTickerString(const char* str2disp, byte tickerSpeed, eColors color) {
  word matrix [16];

  byte strLength = strlen(str2disp);
  unsigned int bufLen;
  char actChar;
  char lastChar;
  byte offsetV = 2;
  bool finish = false;
  unsigned int i = 0;
  unsigned int temp_shift = 0;

  while (!finish) {
    renderer.clearScreenBuffer(matrix);
    lastChar = 'W';
    unsigned int shift = 0; // Schiebekorrektur aufgrund variierender Buchstabenbreite
    for (byte k = 0; k < strLength; k++) {
      actChar = str2disp[k];
      if (actChar == ' ') {
        shift += 3;  //bei einem Space eine Lücke von:
      }
      else {
        shift -= stabenBig[lastChar - '!'][7];
        for (byte j = 0; j < 7; j++) {
          temp_shift = (1 - shift + i);
          if(temp_shift < 16){
            matrix[offsetV + j] |= (stabenBig[actChar - '!'][j] << temp_shift) & 0b1111111111100000;
          }
        }
        if (k < (strLength - 1)) {
          shift += 6; // Max. Buchstabenbreite + ein Pixel Lücke
        }
        lastChar = actChar;
      }
    }
    writeToBuffer(matrix, 3 * (10 - tickerSpeed), color);
    bufLen = shift + 15;
    if (i == bufLen) {
      finish = true;
    }
    else {
      i++;
    }
  }
}

/**
   Intro
*/
void Effects::showIntro(eColors color) {
  word matrix [16];

  renderer.clearScreenBuffer(matrix);
  for (int j = 0; j < 11; j++) {
    for (byte i = 0; i < 10; i++) {
      matrix[i] |= 0b1 << (15 - j);
    }
    writeToBuffer(matrix, 5, color);
  }
  for (int j = 0; j < 11; j++) {
    for (int i = 0; i < 10; i++) {
      matrix[i] ^= 0b1 << (5 + j);
    }
    writeToBuffer(matrix, 5, color);
  }
  renderer.clearScreenBuffer(matrix);
  for (int i = 9; i >= 0; i--) {
    matrix[i] |= 0b1111111111100000;
    writeToBuffer(matrix, 5, color);
  }
  for (int i = 0; i < 10; i++) {
    matrix[i] ^= 0b1111111111100000;
    writeToBuffer(matrix, 5, color);
  }
}

/**
   Pulsierender Herz-Effekt
*/
void Effects::showHeart(eColors color) {
  word matrix [16];
  for (byte y = 0; y < 3; y++) {
    renderer.clearScreenBuffer(matrix);
    for (byte j = 0; j < 8; j++) {
      matrix[1 + j] |= (effectMasksHeart[0][j] << 5);
    }
    writeToBuffer(matrix, 36, color);
    for (byte i = 0; i < 2; i++) {
      renderer.clearScreenBuffer(matrix);
      for (byte z = 0; z < 2; z++) {
        for (byte j = 0; j < 8; j++) {
          matrix[1 + j] |= (effectMasksHeart[z][j] << 5);
        }
        writeToBuffer(matrix, 12, color);
      }
    }
  }
  renderer.clearScreenBuffer(matrix);
  for (byte j = 0; j < 8; j++) {
    matrix[1 + j] |= (effectMasksHeart[0][j] << 5);
  }
  writeToBuffer(matrix, 48, color);
}

/**
   Feuerwerk-Effekt
*/
void Effects::showFireWork(byte posX, eColors color) {
  word matrix [16];

  for (byte i = 9; i >= 3; i--) {
    renderer.clearScreenBuffer(matrix);
    renderer.setPixelInScreenBuffer(posX, i, matrix);
    writeToBuffer(matrix, 7, color);
  }

  for (byte i = 0; i <= 2; i++) {
    renderer.clearScreenBuffer(matrix);
    for (byte j = 0; j < 10; j++) {
      matrix[j] |= (effectMasksFireWork[i][j] << (10 - posX)) & 0b1111111111100000;
    }
    writeToBuffer(matrix, 3 + round(10 * i / 3), color);
  }
  for (byte i = 0; i <= 10; i++) {
    renderer.clearScreenBuffer(matrix);
    for (byte j = 0; j < 10 - i; j++) {
      matrix[j + i] |= (effectMasksFireWork[3 + i % 3][j] << (10 - posX)) & 0b1111111111100000;
    }
    writeToBuffer(matrix, 20, color);
  }
}

/**
   Kerzen-Effekt
*/
void Effects::showCandle(eColors color) {
  word matrix [16];
  for (byte k = 0; k < 5; k++) {
    for (int j = -4; j < 4; j++) {
      renderer.clearScreenBuffer(matrix);
      for (byte i = 5; i < 10; i++) {
        matrix[i] |= (effectMasksCandle[5][i] << 5);
      }
      for (byte i = 0; i < 5; i++) {
        matrix[i] |= (effectMasksCandle[4 - abs(j % 4)][i] << 5);
      }
      writeToBuffer(matrix, 10, color);
    }
  }
}

/**
   BJ-Effekt
*/
void Effects::showBJ(eColors color) {
  word matrix [16];
  for( int i = 0; i < 7; i++){
    for( int y = 0; y < 2; y++){
      renderer.clearScreenBuffer(matrix);
      for (int j = 0; j < 11; j++) {
        matrix[j] |= (effectMasksSnBDay[y][j] << 5);  
      }
      writeToBuffer(matrix, 20, color);
    }
  }
}

/**
   Bitmap
*/
void Effects::showBitmap(byte bitmapIdx, byte duration, eColors color) {
  word matrix [16];
  renderer.clearScreenBuffer(matrix);
  for (byte i = 0; i < 10; i++) {
    for (byte j = 0; j < 11; j++) {
      matrix[i] |= ((bitmaps[bitmapIdx - BITMAP_MIN][j] >> i) & 0x0001) << (15 - j);
    }
  }
  writeToBuffer(matrix, duration, color);
}

/**
   Bitmap-Effekt
*/
void Effects::showAnimatedBitmap(byte animatedBitmap, eColors color) {
  switch (animatedBitmap) {
    case ANI_BITMAP_CHAMPGLASS:
      for (byte i = 0; i < 6; i++) {
        showBitmap(BITMAP_CHAMPGLASS1 + i % 2, 25, color);
      }
      break;
    case ANI_BITMAP_CHRISTTREE:
      for (byte i = 0; i < 4; i++) {
        showBitmap(BITMAP_CHRISTTREE1 + i % 2, 25, color);
      }
      break;
    case ANI_BITMAP_SMILEY_WINK:
      showBitmap(BITMAP_SMILEY, 2 * 25, color);
      showBitmap(BITMAP_SMILEY_WINK, 25, color);
      showBitmap(BITMAP_SMILEY, 25, color);
      break;
    default:
      ;
  }
}

void Effects::writeToBuffer(word aMatrix[], unsigned int aDuration, eColors color)
{
  ledDriver.writeScreenBufferToLEDs(aMatrix, color);
  delay(aDuration * RGB_SPEED_CORRECTION);
}


