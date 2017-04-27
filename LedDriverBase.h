/**
 * LedDriver
 * Abstrakte Klasse fuer einen LED-Treiber. Hiervon koennen andere Implementierungen
 * erben (75HC595, MAX7219, AS1130 etc.)
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.5
 * @created  18.1.2013
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - printSignature() eingefuehrt.
 * V 1.2:  - Setzen und lesen von Pixel eingefuehrt.
 * V 1.3:  - Anpassung auf Helligkeit in Prozent.
 * V 1.4:  - Getter fuer Helligkeit eingefuehrt.
 * V 1.5:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#include "Arduino.h"

class LedDriverBase {
public:
    virtual void init();

    virtual void printSignature();

    virtual void writeScreenBufferToLEDs(word matrix[16], uint8_t color);

    virtual void setBrightness(byte brightness);
    virtual byte getBrightness();

    void setColor(byte red, byte green, byte blue);
    byte getRed();
    byte getGreen();
    byte getBlue();

    virtual void setLinesToWrite(byte linesToWrite);

    virtual void shutDown();
    virtual void wakeUp();

    virtual void clearData();

    void setPixelInScreenBuffer(byte x, byte y, word matrix[16]);
    boolean getPixelFromScreenBuffer(byte x, byte y, word matrix[16]);

private:
    byte _red, _green, _blue;
};

#endif
