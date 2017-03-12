/******************************************************************************
   Renderer.h
******************************************************************************/

#ifndef RENDERER_H
#define RENDERER_H

#include "Arduino.h"
#include "Woerter_DE.h"
#include "Woerter_DE_MKF.h"
#include "Woerter_D3.h"
#include "Woerter_CH.h"
#include "Woerter_EN.h"
#include "Woerter_ES.h"
#include "Woerter_FR.h"
#include "Woerter_IT.h"
#include "Woerter_NL.h"
#include "Languages.h"
#include "Staben.h"
#include "Zahlen.h"

class Renderer {
  public:
    enum eTextPos : uint8_t {
      TEXT_POS_TOP,
      TEXT_POS_MIDDLE = 2,
      TEXT_POS_BOTTOM = 5
    };

    Renderer();
    void setHours(uint8_t hours, boolean glatt, uint8_t language, word matrix[16]);
    void setTime(uint8_t hours, uint8_t minutes, uint8_t language, word matrix[16]);
    void setCorners(uint8_t minutes, word matrix[16]);
    void activateAlarmLed(word matrix[16]);
	void deactivateAlarmLed(word matrix[16]);
    void clearEntryWords(uint8_t language, word matrix[16]);
    void setSmallText(String menuText, eTextPos textPos, word matrix[16]);
    void setAMPM(uint8_t hours, uint8_t language, word matrix[16]);
    void setPixelInScreenBuffer(uint8_t x, uint8_t y, word matrix[16]);
    void unsetPixelInScreenBuffer(uint8_t x, uint8_t y, word matrix[16]);
    void clearScreenBuffer(word matrix[16]);
    void setAllScreenBuffer(word matrix[16]);
    
  private:
    boolean isNumber(char symbol);

    void FR_hours(uint8_t hours, word matrix[16]);
    void IT_hours(uint8_t hours, word matrix[16]);
    void ES_hours(uint8_t hours, word matrix[16]);
};

#endif
