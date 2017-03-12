/******************************************************************************
   Debug.cpp
******************************************************************************/

#include "Debug.h"

Debug::Debug() {
}

// Screenbuffer in die Konsole schreiben
void Debug::debugScreenBuffer(word ScreenBuffer[]) {
	const char buchstabensalat[][17] = {
	  {"ESKISTAFUNF2"},
	  {"ZEHNZWANZIG1"},
	  {"DREIVIERTEL4"},
	  {"VORFUNKNACH3"},
	  {"HALBAELFUNFA"},
	  {"EINSXAMZWEI_"},
	  {"DREIPMJVIER_"},
	  {"SECHSNLACHT_"},
	  {"SIEBENZWOLF_"},
	  {"ZEHNEUNKUHR_"}
	};
	Serial.println("\033[0;0H"); // Set cursor to 0, 0 position in console
	Serial.println(" -----------");
	for (uint8_t zeile = 0; zeile < 10; zeile++) {
		word leds = ScreenBuffer[zeile];
		char spalte[16];
		for (int8_t i = 15; i >= 0; i--) {
			spalte[i] = ((leds & 1) ? buchstabensalat[zeile][i] : ' ');
			leds = leds >> 1;
		}
		Serial.print('|');
		for (uint8_t i = 0; i < 11; i++) Serial.print(spalte[i]);
		Serial.print('|');
		Serial.println(spalte[11]); // Corner LEDs
	}
	Serial.println(" -----------");
}

// Zeit in die Konsole schreiben
void Debug::debugTime(String label, time_t time) {
	Serial.print(label);
	Serial.printf(" %02d:%02d:%02d %02d.%02d.%04d\n\r", hour(time), minute(time), second(time), day(time), month(time), year(time));
}

void Debug::debugFps() {
	frames++;
	if (millis() - lastFpsCheck > 1000) {
		Serial.print("FPS: ");
		Serial.println(frames);
		lastFpsCheck = millis();
		frames = 0;
	}
}