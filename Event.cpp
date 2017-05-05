/**
* Event.cpp
* Klasse für ein jährliches Event
*
* @mc       Arduino/UNO
* @autor    Manuel Bracher / manuel.bracher@gmail.com
* @version  1.0
* @created  02.01.15
*
* Versionshistorie:
* V 1.0:  - Erstellt.
*/

#include "Event.h"

// #define DEBUG
#include "Debug.h"

Event::Event(byte month,
             byte date,
             const char* txt,
             Effects::eEffects effect,
             eColors color) :
  _month (month), _date (date),
  _txt (txt), _effect (effect),
  _color(color) {};

void Event::show() {
  DEBUG_PRINT(F("Ticker String: "));
  DEBUG_PRINTLN(_txt);
  if (strlen(_txt) != 0)
    Effects::showTickerString(_txt, TICKER_SPEED, (eColors)settings.getColor());
  if (_effect < Effects::BITMAP_MIN) {
    switch (_effect)
    {
      case Effects::NO_EFFECT:
        break;
      case Effects::EFFECT_FIREWORK:
        Effects::showFireWork(5, _color);
        Effects::showFireWork(2, _color);
        Effects::showFireWork(8, _color);
        break;
      case Effects::EFFECT_HEART:
        Effects::showHeart(_color);
        break;
      case Effects::EFFECT_CANDLE:
        Effects::showCandle(_color);
        break;
      case Effects::EFFECT_SNBD:
        Effects::showBJ(_color);
        break;

      default:
        ;
    }
  }
  if ((_effect >= Effects::BITMAP_MIN) && (_effect < Effects::ANI_BITMAP_MIN)) {
    Effects::showBitmap(_effect, 225, _color);
  }
  if (_effect >= Effects::ANI_BITMAP_MIN) {
    Effects::showAnimatedBitmap(_effect, _color);
  }
}

byte Event::getMonth() {
  return _month;
}

byte Event::getDate() {
  return _date;
}
