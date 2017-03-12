/******************************************************************************
  Settings.cpp
******************************************************************************/

#include "Settings.h"

Settings::Settings() {
	loadFromEEPROM();
}

uint8_t Settings::getLanguage() {
	return mySettings.language;
}

void Settings::setLanguage(uint8_t language) {
	mySettings.language = language;
}

boolean Settings::getUseLdr() {
	return mySettings.useLdr;
}

void Settings::toggleUseLdr() {
	mySettings.useLdr = !mySettings.useLdr;
}

int16_t Settings::getBrightness() {
	return mySettings.brightness;
}

void Settings::setBrightness(int16_t ledBrightness) {
	mySettings.brightness = ledBrightness;
}

void Settings::setColor(uint8_t color) {
	mySettings.color = color;
}

uint8_t Settings::getColor() {
	return mySettings.color;
}

uint8_t Settings::getTimeout() {
	return mySettings.timeout;
}

void Settings::setTimeout(uint8_t timeout) {
	mySettings.timeout = timeout;
}

boolean Settings::getEsIst() {
	return mySettings.esIst;
}

void Settings::toggleEsIst() {
	mySettings.esIst = !mySettings.esIst;
}

boolean Settings::getAlarm1() {
	return mySettings.alarm1;
}

void Settings::toggleAlarm1() {
	mySettings.alarm1 = !mySettings.alarm1;
}

time_t Settings::getAlarmTime1() {
	return mySettings.alarmTime1;
}

void Settings::setAlarmTime1(time_t alarmTime) {
	mySettings.alarmTime1 = alarmTime;
}

boolean Settings::getAlarm2() {
	return mySettings.alarm2;
}

void Settings::toggleAlarm2() {
	mySettings.alarm2 = !mySettings.alarm2;
}

time_t Settings::getAlarmTime2() {
	return mySettings.alarmTime2;
}

void Settings::setAlarmTime2(time_t alarmTime) {
	mySettings.alarmTime2 = alarmTime;
}

time_t Settings::getNightOffTime() {
	return mySettings.nightOffTime;
}

void Settings::setNightOffTime(time_t nightOffTime) {
	mySettings.nightOffTime = nightOffTime;
}

time_t Settings::getNightOnTime() {
	return mySettings.nightOnTime;
}

void Settings::setNightOnTime(time_t nightOnTime) {
	mySettings.nightOnTime = nightOnTime;
}


// Setzt alle Werte auf Defauleinstellungen
void Settings::resetToDefault() {
	mySettings.magicNumber = SETTINGS_MAGIC_NUMBER;
	mySettings.version = SETTINGS_VERSION;
	mySettings.language = 0;
	mySettings.useLdr = false;
	mySettings.brightness = 128;
	mySettings.color = 0;
	mySettings.timeout = 5;
	mySettings.esIst = true;
	mySettings.alarm1 = false;
	mySettings.alarmTime1 = 0;
	mySettings.alarm2 = false;
	mySettings.alarmTime2 = 0;
	mySettings.nightOffTime = 0;
	mySettings.nightOnTime = 0;
	saveToEEPROM();
}

// Einstellungen laden
void Settings::loadFromEEPROM() {
	EEPROM.begin(512);
	EEPROM.get(0, mySettings);
	if ((mySettings.magicNumber != SETTINGS_MAGIC_NUMBER) || (mySettings.version != SETTINGS_VERSION)) resetToDefault();
}

// Einstellungen speichern
void Settings::saveToEEPROM() {
	EEPROM.begin(512);
	EEPROM.put(0, mySettings);
	EEPROM.commit();
}
