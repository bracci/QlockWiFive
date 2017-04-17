/******************************************************************************
   Modes.h
******************************************************************************/

#ifndef MODES_H
#define MODES_H

#include <Arduino.h>
#include "Configuration.h"

typedef enum eMode : uint8_t {
	STD_MODE_NORMAL,
	STD_MODE_AMPM,
	STD_MODE_SECONDS,
	STD_MODE_DATE,
#ifdef RTC_BACKUP
	STD_MODE_TEMP,
#endif
	STD_MODE_SET_TIMER,
	STD_MODE_TIMER,
	STD_MODE_ALARM_1,
	STD_MODE_SET_ALARM_1,
	STD_MODE_ALARM_2,
	STD_MODE_SET_ALARM_2,
  STD_MODE_BLANK,
	STD_MODE_COUNT,
	EXT_MODE_START,
	EXT_MODE_TEXT_MAIN = EXT_MODE_START,
#ifdef LDR
	EXT_MODE_LDR,
#endif
	EXT_MODE_BRIGHTNESS,
	EXT_MODE_COLOR,
	EXT_MODE_TIMEOUT,
	EXT_MODE_LANGUAGE,
	EXT_MODE_TEXT_TIME,
	EXT_MODE_TIMESET,
	EXT_MODE_IT_IS,
	EXT_MODE_DAYSET,
	EXT_MODE_MONTHSET,
	EXT_MODE_YEARSET,
	EXT_MODE_TEXT_NIGHTOFF,
	EXT_MODE_NIGHTOFF,
	EXT_MODE_TEXT_NIGHTON,
	EXT_MODE_NIGHTON,
	EXT_MODE_TEXT_TEST,
	EXT_MODE_TEST,
	EXT_MODE_COUNT
} Mode;

// Overload the ControlType++ operator
inline Mode& operator++(Mode& eDOW, int) {
	const uint8_t i = static_cast<uint8_t>(eDOW) + 1;
	eDOW = static_cast<Mode>((i) % EXT_MODE_COUNT);
	return eDOW;
}

#endif
