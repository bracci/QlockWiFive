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
#include "LedDriverNeoPixel.h"
#include "Configuration.h"

// #define DEBUG
#include "Debug.h"

#ifdef MATRIX_XXL
#define NUM_PIXEL 225
#else
#define NUM_PIXEL 114
#endif

/**
 * Initialisierung.
 *
 * @param data Pin, an dem die Data-Line haengt.
 */
LedDriverNeoPixel::LedDriverNeoPixel(byte dataPin) {
    _dataPin = dataPin;
    _strip = new Adafruit_NeoPixel(NUM_PIXEL, _dataPin, NEO_GRBW + NEO_KHZ800);
    _strip->begin();
    setColor(250, 255, 200);
}

/**
 * init() wird im Hauptprogramm in init() aufgerufen.
 * Hier sollten die LED-Treiber in eine definierten
 * Ausgangszustand gebracht werden.
 */
void LedDriverNeoPixel::init() {
    setBrightness(50);
    clearData();
    wakeUp();
}

void LedDriverNeoPixel::printSignature() {
    Serial.println(F("NeoPixel - WS2812B"));
}

/**
 * Den Bildschirm-Puffer auf die LED-Matrix schreiben.
 *
 * @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
 *                  FALSE, wenn es ein Refresh-Aufruf war.
 */
void LedDriverNeoPixel::writeScreenBufferToLEDs(word matrix[16], uint8_t color) {
  _strip->clear();
  
  for (byte y = 0; y < 10; y++) {
      for (byte x = 5; x < 16; x++) {
          word t = 1 << x;
          if ((matrix[y] & t) == t) {
              _setPixel(15 - x, y, color);
          }
      }
  }

  byte cornerLedCount[] = { 1, 0, 3, 2, 4 };
  for (uint8_t i = 0; i < 5; i++) if ((matrix[cornerLedCount[i]] & 0b0000000000010000) > 0) _setPixel(110 + i, color);
  
  _strip->show();
}

/**
 * Die Helligkeit des Displays anpassen.
 *
 * @param brightnessInPercent Die Helligkeit.
 */
void LedDriverNeoPixel::setBrightness(byte brightness) {
    if (brightness != _brightness) {
        _brightness = brightness;
    }
}

/**
 * Die aktuelle Helligkeit bekommen.
 */
byte LedDriverNeoPixel::getBrightness() {
    return _brightness;
}

/**
 * Anpassung der Groesse des Bildspeichers.
 *
 * @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
 *                     geschrieben werden?
 */
void LedDriverNeoPixel::setLinesToWrite(byte linesToWrite) {
}

/**
 * Das Display ausschalten.
 */
void LedDriverNeoPixel::shutDown() {
    _strip->clear();
    _strip->show();
}

/**
 * Das Display einschalten.
 */
void LedDriverNeoPixel::wakeUp() {
}

/**
 * Den Dateninhalt des LED-Treibers loeschen.
 */
void LedDriverNeoPixel::clearData() {
    _strip->clear();
    _strip->show();
}

/**
 * Einen X/Y-koordinierten Pixel in der Matrix setzen.
 */
void LedDriverNeoPixel::_setPixel(byte x, byte y, uint32_t c) {
#ifdef LED_LAYOUT_HORIZONTAL
  _setPixel(x + (y * 11), c);
#endif
#ifdef LED_LAYOUT_VERTICAL
  _setPixel(y + (x * 10), c);
#endif
}

/**
 * Einen Pixel im Streifen setzten (die Eck-LEDs sind am Ende).
 */
void LedDriverNeoPixel::_setPixel(byte num, uint8_t color) {
  uint8_t red = defaultColors[color].red;
  uint8_t green = defaultColors[color].green;
  uint8_t blue = defaultColors[color].blue;
  uint32_t c = _strip->Color(_brightnessScaleColor(red), _brightnessScaleColor(green), _brightnessScaleColor(blue));
  
  #ifdef MATRIX_XXL
    uint16_t ledNum;
    if (num < 110) {
      if ((num / 10) % 2 == 0) {
        ledNum = num * 2;
        _strip->setPixelColor(ledNum, c);
        ledNum++;
        _strip->setPixelColor(ledNum, c);
      }
      else {
        ledNum = (((num / 10) * 10) + 9 - (num % 10)) * 2;
        _strip->setPixelColor(ledNum, c);
        ledNum++;
        _strip->setPixelColor(ledNum, c);
      }
    }
    else {
      switch (num) {
      case 110:
        _strip->setPixelColor(222, c);
        break;
      case 111:
        _strip->setPixelColor(221, c);
        break;
      case 112:
        _strip->setPixelColor(220, c);
        break;
      case 113:
        _strip->setPixelColor(223, c);
        break;
      case 114:
        _strip->setPixelColor(224, c);
        break;
      default:
        break;
      }
    }
  #else
    if (num < 110) {
        if ((num / 11) % 2 == 0) {
            _strip->setPixelColor(num, c);
        } else {
            _strip->setPixelColor(((num / 11) * 11) + 10 - (num % 11), c);
        }
    } else {
        switch (num) {
            case 110:
                _strip->setPixelColor(111, c);
                break;
            case 111:
                _strip->setPixelColor(112, c);
                break;
            case 112:
                _strip->setPixelColor(113, c);
                break;
            case 113:
                _strip->setPixelColor(110, c);
                break;
        }
    }
  #endif
}

/**
 * Funktion fuer saubere 'Regenbogen'-Farben.
 * Kopiert aus den Adafruit-Beispielen (strand).
 */
uint32_t LedDriverNeoPixel::_wheel(byte wheelPos) {
    if (wheelPos < 85) {
        return _strip->Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    } else if (wheelPos < 170) {
        wheelPos -= 85;
        return _strip->Color(255 - wheelPos * 3, 0, wheelPos * 3);
    } else {
        wheelPos -= 170;
        return _strip->Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
}

/**
 * Hilfsfunktion fuer das Skalieren der Farben.
 */
byte LedDriverNeoPixel::_brightnessScaleColor(byte colorPart) {
    return map(_brightness, 0, 255, 0, colorPart);
}

void LedDriverNeoPixel::show() {
  _strip->show();
}
