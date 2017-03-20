/******************************************************************************
   LedDriver.h
******************************************************************************/

#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include "Arduino.h"
#include "Colors.h"
#include "Configuration.h"

#ifdef LED_RGB
#define NUM_LEDS 115
#endif

#ifdef LED_RGBW
#define NUM_LEDS 230
#endif

class LedDriver {
public:
	LedDriver();
	void writeScreenBufferToLEDs(word matrix[16], uint8_t color);
	void clear();
	void setBrightness(uint8_t ledBrightness);
	void getBrightness();
	void show();
	void setPixel(uint8_t num, uint8_t color);

private:
	void setPixel(uint8_t x, uint8_t y, uint8_t color);
	uint8_t getWhite(uint8_t red, uint8_t green, uint8_t blue);
	CRGB leds[NUM_LEDS];
};

#endif
