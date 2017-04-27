/**
 * LedDriverNeoPixel
 * Implementierung auf der Basis von WS2812B-Streifen wie sie die Adafruit-Neo-Pixel verwenden.
 *
 * Es lohnt sich in jedem Fall, den UeberGuide von Adafruit zu lesen:
 * https://learn.adafruit.com/adafruit-neopixel-uberguide/overview
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.2
 * @created  5.1.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Getter fuer Helligkeit nachgezogen.
 * V 1.2:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 *
 * Verkabelung: Einspeisung oben links, dann schlangenfoermig runter,
 * dann Ecke unten links, oben links, oben rechts, unten rechts.
 *
 */
#ifndef LED_DRIVER_NEOPIXEL_H
#define LED_DRIVER_NEOPIXEL_H

#include "Arduino.h"
#include "LedDriverBase.h"
#include "Colors.h"
#include <Adafruit_NeoPixel_RGBW.h>

class LedDriverNeoPixel : public LedDriverBase {
public:
    LedDriverNeoPixel(byte dataPin);

    void init();

    void printSignature();

    void writeScreenBufferToLEDs(word matrix[16], uint8_t color);

    void setBrightness(byte brightness);
    byte getBrightness();

    void setLinesToWrite(byte linesToWrite);

    void shutDown();
    void wakeUp();

    void show();

    void clearData();

private:
    byte _brightness;

    void _setPixel(byte x, byte y, uint32_t c);
    void _setPixel(byte num, uint8_t color);

    uint32_t _wheel(byte wheelPos);

    byte _brightnessScaleColor(byte colorPart);

    byte _dataPin;
    Adafruit_NeoPixel_RGBW *_strip;
};

#endif
