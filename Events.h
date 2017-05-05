/**
 * Events.h
 * Instanzierung jährlicher Events
 *
 * @mc       Arduino/UNO
 * @autor    Manuel Bracher / manuel.bracher@gmail.com
 * @version  1.0
 * @created  02.01.15
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */
#ifndef EVENTS_H
#define EVENTS_H

#include "Event.h"
#include "Colors.h"

/**************************************************************************************************************
* Instanzierung und Initialisierung jährlicher Events. Ein Event enthält eine Laufschrift, gefolgt
* von einem Bildeffekt (Herz, Feuerwerk, etc.). Diese werden am definierten Jahrestag jeweils zur vollen Stunde
* während einer definierten Dauer nonstop angezeigt. Soll nur eine Laufschrift (ohne Bildeffekt) angezeigt werden,
* ist als Effekt-Input NO_EFFECT zu wählen. Soll nur ein Bildeffekt (ohne Laufschrift) gezeigt werden, ist als
* Text-Input "" zu definieren. (Monat, Tag, Text, Effekt, Effektfarbe) Das Event-Array kann "beliebig" ergänzt
* werden.
**************************************************************************************************************/
Event events[] = {
  Event(1, 1, "Happy New Year", Effects::EFFECT_FIREWORK, White),
  Event(2, 14, "Happy Valentine's Day", Effects::EFFECT_HEART, Red),
  Event(3, 14, "Happy Steak n BJ Day", Effects::EFFECT_SNBD, White),
  Event(5, 6, "Aues Guete zur Hochzyt", Effects::EFFECT_HEART, Red),
};

//NO_EFFECT = 255,
//EFFECT_FIREWORK = 0,
//EFFECT_HEART,
//EFFECT_CANDLE,
//EFFECT_LOVEU,
//EFFECT_INTRO,
//EFFECT_SNBD,
//
//BITMAP_MIN,
//BITMAP_HEART = BITMAP_MIN,
//BITMAP_DOTCIRCLE,
//BITMAP_WEDDINGRING,
//BITMAP_BDPIE,
//BITMAP_CHAMPGLASS1,
//BITMAP_CHAMPGLASS2,
//BITMAP_CHRISTTREE1,
//BITMAP_CHRISTTREE2,
//BITMAP_SMILEY,
//BITMAP_SMILEY_WINK,
//
//ANI_BITMAP_MIN,
//ANI_BITMAP_CHAMPGLASS = ANI_BITMAP_MIN,
//ANI_BITMAP_CHRISTTREE,
//ANI_BITMAP_SMILEY_WINK,


/**************************************************************************************************************
 * Anzahl definierter Events
 **************************************************************************************************************/
int nbrOfEvts = sizeof(events) / sizeof(Event);

const String sEvtRep[] = {
  "5",   // 00
  "15", // 01
  "30", // 02
  "60", // 03
};

enum eEvtRep : uint8_t {
  EVT_REP_5,
  EVT_REP_15,
  EVT_REP_30,
  EVT_REP_60,
  EVT_REP_COUNT
};

#endif // EVENTS_H
