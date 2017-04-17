/******************************************************************************
QLOCKWORK
Eine Firmware der Selbstbau-QLOCKTWO.

@mc      ESP8266
@created 01.02.2017
******************************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <Syslog.h>
#include "Button.h"
#include "Configuration.h"
#include "Debug.h"
#include "Modes.h"
#include "Renderer.h"
#include "Colors.h"
#include "Languages.h"
#include "LedDriver.h"
#include "Settings.h"

#define FIRMWARE_VERSION "qw20170401"

/******************************************************************************
Init
******************************************************************************/

Renderer renderer;
LedDriver ledDriver;
Settings settings;
WiFiUDP Udp;
ESP8266WebServer server(80);
Debug debug;
#ifdef RTC_BACKUP
RtcDS3231<TwoWire> Rtc(Wire);
#endif
#ifdef IR_REMOTE
IRrecv irrecv(PIN_IR_RECEIVER);
#endif
#ifdef SYSLOG_SERVER
Syslog syslog(Udp, SYSLOG_SERVER, SYSLOG_PORT, HOSTNAME, "", SYSLOG_FACILITY);
#endif
#ifdef BUTTONS
Button minusButton(PIN_M_PLUS, BUTTONS_PRESSING_AGAINST);
Button plusButton(PIN_H_PLUS, BUTTONS_PRESSING_AGAINST);
Button extModeDoubleButton(PIN_M_PLUS, PIN_H_PLUS, BUTTONS_PRESSING_AGAINST);
Button modeButton(PIN_MODE, BUTTONS_PRESSING_AGAINST);
#endif

word matrix[16];
boolean ScreenBufferNeedsUpdate = true;
IPAddress timeServerIP;
uint8_t packetBuffer[48];
Mode mode = STD_MODE_NORMAL;
Mode lastMode = mode;
uint8_t fallBackCounter = 0;
uint8_t lastDay = 0;
uint8_t lastHour = 0;
uint8_t lastMinute = 0;
uint8_t lastFiveMinute = 0;
boolean timerSet = false;
uint16_t timerMinutes = 0;
time_t timer = 0;
time_t lastTime = 0;
uint8_t alarmOn = 0;
uint8_t testColumn = 0;
uint32_t lastBrightnessCheck = 0;
uint8_t ledBrightness = 0;
uint8_t lv = 0;
uint16_t minBrightness = 1023;
uint16_t maxBrightness = 0;
uint16_t lastValue = 0;
int8_t maxColor = sizeof(defaultColors) / 3 - 1;
#ifdef IR_REMOTE
decode_results irDecodeResults;
#endif

#ifdef TIMEZONE_PST
TimeChangeRule usPDT = { "PDT", Second, dowSunday, Mar, 2, -420 };
TimeChangeRule usPST = { "PST", First, dowSunday, Nov, 2, -480 };
Timezone timeZone(usPDT, usPST);
#endif
#ifdef TIMEZONE_MST
TimeChangeRule usMDT = { "MDT", Second, dowSunday, Mar, 2, -360 };
TimeChangeRule usMST = { "MST", First, dowSunday, Nov, 2, -420 };
Timezone timeZone(usMDT, usMST);
#endif
#ifdef TIMEZONE_CST
TimeChangeRule usCDT = { "CDT", Second, dowSunday, Mar, 2, -300 };
TimeChangeRule usCST = { "CST", First, dowSunday, Nov, 2, -360 };
Timezone timeZone(usCDT, usCST);
#endif
#ifdef TIMEZONE_EST
TimeChangeRule usEDT = { "EDT", Second, Sun, Mar, 2, -240 };
TimeChangeRule usEST = { "EST", First, Sun, Nov, 2, -300 };
Timezone timeZone(usEDT, usEST);
#endif
#ifdef TIMEZONE_GMT
TimeChangeRule BST = { "BST", Last, Sun, Mar, 1, 60 };
TimeChangeRule GMT = { "GMT", Last, Sun, Oct, 2, 0 };
Timezone timeZone(BST, GMT);
#endif
#ifdef TIMEZONE_CET
TimeChangeRule CEST = { "CEST", Last, Sun, Mar, 2, 120 };
TimeChangeRule CET = { "CET", Last, Sun, Oct, 3, 60 };
Timezone timeZone(CEST, CET);
#endif
#ifdef TIMEZONE_AEST
TimeChangeRule aEDT = { "AEDT", First, Sun, Oct, 2, 660 };
TimeChangeRule aEST = { "AEST", First, Sun, Apr, 3, 600 };
Timezone timeZone(aEDT, aEST);
#endif

/******************************************************************************
setup()
******************************************************************************/

void setup() {

	// Seriellen Port initialisieren
	Serial.begin(SERIAL_SPEED);
	delay(1000);

	DEBUG_PRINTLN();
	DEBUG_PRINTLN("QLOCKWORK");
	DEBUG_PRINT("Firmware: ");
	DEBUG_PRINTLN(FIRMWARE_VERSION);

	// LDR, Buzzer und LED initialisieren
	DEBUG_PRINTLN("Stetting up LDR, Buzzer and LED.");
	pinMode(PIN_LDR, INPUT);
	pinMode(PIN_BUZZER, OUTPUT);
#ifdef BOARD_LED
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, HIGH);
#endif

#ifdef IR_REMOTE
	// IR-Empfaenger initialisieren
	DEBUG_PRINTLN("Starting IR receiver.");
	irrecv.enableIRIn();
#endif

	// WiFi und Dienste initialisieren
	renderer.clearScreenBuffer(matrix);
	matrix[0] = 0b0000000000000000;
	matrix[1] = 0b0000111000000000;
	matrix[2] = 0b0011111110000000;
	matrix[3] = 0b1111111111100000;
	matrix[4] = 0b0111111111000000;
	matrix[5] = 0b0011111110000000;
	matrix[6] = 0b0001111100000000;
	matrix[7] = 0b0000111000000000;
	matrix[8] = 0b0000010000000000;
	matrix[9] = 0b0000000000000000;
	ledBrightness = settings.getBrightness();
	ledDriver.setBrightness(100);
	ledDriver.writeScreenBufferToLEDs(matrix, 0); // Color 0: weiss
	delay(1000);

  WiFi.hostname(HOSTNAME);
	WiFiManager wifiManager;
	//wifiManager.resetSettings();
	wifiManager.setTimeout(60);
	wifiManager.autoConnect(HOSTNAME);
	if (WiFi.status() != WL_CONNECTED) {
		DEBUG_PRINTLN("Error connecting to WiFi. Shutting down WiFi.");
		renderer.clearScreenBuffer(matrix);
		renderer.setSmallText("ER", Renderer::TEXT_POS_TOP, matrix);
		renderer.setSmallText("OR", Renderer::TEXT_POS_BOTTOM, matrix);
		ledDriver.writeScreenBufferToLEDs(matrix, 1); // Color 1: rot
		WiFi.mode(WIFI_OFF);
		digitalWrite(PIN_BUZZER, HIGH);
		delay(1500);
		digitalWrite(PIN_BUZZER, LOW);
	}
	else {
		DEBUG_PRINTLN("WiFi connected.");
		renderer.clearScreenBuffer(matrix);
		renderer.setSmallText("OK", Renderer::TEXT_POS_MIDDLE, matrix);
		ledDriver.writeScreenBufferToLEDs(matrix, 2); // Color 2: gruen
		for (uint8_t i = 0; i < 3; i++) {
			digitalWrite(PIN_BUZZER, HIGH);
			delay(100);
			digitalWrite(PIN_BUZZER, LOW);
			delay(100);
		}
	}
	DEBUG_PRINTLN("Starting mDNS responder.");
	MDNS.begin(HOSTNAME);
	DEBUG_PRINTLN("Starting WebServer on Port 80.");
	setupWebServer();
	DEBUG_PRINTLN("Starting UDP on Port 2390.");
	Udp.begin(2390);
	DEBUG_PRINTLN("Starting Arduino-OTA service.");
	ArduinoOTA.setPassword((const char*)OTA_PASS);
	ArduinoOTA.begin();
	DEBUG_PRINT("Free RAM: ");
	DEBUG_PRINTLN(system_get_free_heap_size());
#ifdef SYSLOG_SERVER
	syslog.log(LOG_INFO, "Firmware: " + String(FIRMWARE_VERSION));
	syslog.log(LOG_DEBUG, "Free RAM: " + String(system_get_free_heap_size()));
#endif

	// Zeitprovider setzen
#ifdef RTC_BACKUP
	Rtc.Begin();
	Rtc.SetIsRunning(true);
	if (WiFi.status() == WL_CONNECTED) {
		DEBUG_PRINTLN("Setting ESP from NTP with RTC backup.");
		setSyncProvider(getNtpTime);
		setSyncInterval(3600);
	}
	else {
		DEBUG_PRINTLN("Setting ESP from RTC.");
		setSyncProvider(getRtcTime);
		setSyncInterval(3600);
	}
#else
	if (WiFi.status() == WL_CONNECTED) {
		DEBUG_PRINTLN("Setting ESP from NTP.");
		setSyncProvider(getNtpTime);
		setSyncInterval(3600);
	}
	else DEBUG_PRINTLN("No provider for setting the time found.");
#endif

	// sonstiges
	lastDay = day();
	lastHour = hour();
	lastFiveMinute = minute() / 5;
	lastMinute = minute();
	lastTime = now();
}

/******************************************************************************
loop()
******************************************************************************/

void loop() {

#ifdef DEBUG_FPS
	debug.debugFps();
#endif

	// Jeden Tag ausfuehren
	if (day() != lastDay) {
		lastDay = day();
		DEBUG_PRINTLN("Neuen Tag erreicht.");
#ifdef SYSLOG_SERVER
		syslog.log(LOG_DEBUG, "Free RAM: " + String(system_get_free_heap_size()));
#endif
		//if (settings.getColor() < maxColor) settings.setColor(settings.getColor() + 1);
		//else settings.setColor(0);
	}

	// Jede Stunde ausfuehren
	if (hour() != lastHour) {
		lastHour = hour();
		DEBUG_PRINTLN("Volle Stunde erreicht.");
#if defined(RTC_BACKUP) && defined(SYSLOG_SERVER)
		syslog.log(LOG_INFO, "Temperature: " + String(Rtc.GetTemperature().AsFloat() + RTC_TEMP_OFFSET) + "C / " + String((Rtc.GetTemperature().AsFloat() + RTC_TEMP_OFFSET) * 1.8 + 32) + "F");
#endif
	}

	// Alle fuenf Minuten ausfuehren
	// lastFiveMinute ist vom Typ uint8_t. Dadurch werden die Nachkommastellen verworfen.
	if ((minute() / 5) != lastFiveMinute) {
		lastFiveMinute = minute() / 5;
		DEBUG_PRINTLN("Volle fuenf Minuten erreicht.");
	}

	// Jede Minute ausfuehren
	if (minute() != lastMinute) {
		lastMinute = minute();
#ifdef DEBUG
		debug.debugTime("Time:", now());
#endif
		if (mode == STD_MODE_NORMAL) ScreenBufferNeedsUpdate = true;
	}

	// Jede Sekunde ausfuehren
	if (now() != lastTime) {
		lastTime = now();

		// Fallback zur Zeitanzeige runterzaehlen
		if (fallBackCounter > 1) fallBackCounter--;
		else if (fallBackCounter == 1) {
			fallBackCounter = 0;
			setMode(STD_MODE_NORMAL);
		}

		// Display braucht Sekundenupdate
		switch (mode) {
		case STD_MODE_SECONDS:
#ifdef RTC_BACKUP
		case STD_MODE_TEMP:
#endif
		case STD_MODE_SET_TIMER:
		case STD_MODE_TIMER:
		case STD_MODE_ALARM_1:
		case STD_MODE_SET_ALARM_1:
		case STD_MODE_ALARM_2:
		case STD_MODE_SET_ALARM_2:
#ifdef LDR
		case EXT_MODE_LDR:
#endif
		case EXT_MODE_COLOR:
		case EXT_MODE_TIMEOUT:
		case EXT_MODE_LANGUAGE:
		case EXT_MODE_TIMESET:
		case EXT_MODE_IT_IS:
		case EXT_MODE_DAYSET:
		case EXT_MODE_MONTHSET:
		case EXT_MODE_YEARSET:
		case EXT_MODE_NIGHTOFF:
		case EXT_MODE_NIGHTON:
		case EXT_MODE_TEST:
			ScreenBufferNeedsUpdate = true;
			break;
		default:
			break;
		}

#ifdef BOARD_LED
		// LED auf dem Board blinken lassen
		if (digitalRead(PIN_LED) == LOW) digitalWrite(PIN_LED, HIGH);
		else digitalWrite(PIN_LED, LOW);
#endif

		// Alarm
		if (settings.getAlarm1() && (hour() == hour(settings.getAlarmTime1())) && (minute() == minute(settings.getAlarmTime1())) && (second() == 0)) alarmOn = 60;
		if (settings.getAlarm2() && (hour() == hour(settings.getAlarmTime2())) && (minute() == minute(settings.getAlarmTime2())) && (second() == 0)) alarmOn = 60;

		// Timer
		if (timerSet && (now() == timer)) {
			setMode(STD_MODE_SET_TIMER);
			timerMinutes = 0;
			timerSet = false;
			alarmOn = 60;
		}

		// ggf. Laerm machen
		if (alarmOn) {
			if (second() % 2 == 0) digitalWrite(PIN_BUZZER, HIGH);
			else digitalWrite(PIN_BUZZER, LOW);
			alarmOn--;
		}
		else digitalWrite(PIN_BUZZER, LOW);

		// Nachtmodus ein/aus
		if ((hour() == hour(settings.getNightOffTime())) && (minute() == minute(settings.getNightOffTime())) && (second() == 0)) setMode(STD_MODE_BLANK);
		if ((hour() == hour(settings.getNightOnTime())) && (minute() == minute(settings.getNightOnTime())) && (second() == 0)) setMode(lastMode);
	}

	// Immer ausfuehren

	// HTTP- und OTA-Anfragen entgegennehmen
	server.handleClient();
	ArduinoOTA.handle();

	// LDR ggf. lesen und Helligkeit einstellen
#ifdef LDR
	if (!settings.getUseLdr()) ledBrightness = settings.getBrightness();
	else {
		if (millis() > (lastBrightnessCheck + 25)) {
			uint16_t val = (1023 - analogRead(PIN_LDR));
			if ((val > (lastValue + 25)) || (val < (lastValue - 25))) {
				if (val < minBrightness) minBrightness = val;
				if (val > maxBrightness) maxBrightness = val + 1;
				lv = map(val, minBrightness, maxBrightness, 0, 255);
				lastValue = val;
			}
			if (ledBrightness < lv) ledBrightness++;
			if (ledBrightness > lv) ledBrightness--;
			lastBrightnessCheck = millis();
		}
	}
#else 
	ledBrightness = settings.getBrightness();
#endif
	ledDriver.setBrightness(constrain(ledBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
	ledDriver.show();

#ifdef IR_REMOTE
	// IR-Empfaenger abfragen und ggf. reagieren
	if (irrecv.decode(&irDecodeResults)) {
		DEBUG_PRINT("IR signal: ");
		DEBUG_PRINTLN(irDecodeResults.value);
		remoteAction(irDecodeResults.value);
		irrecv.resume();
	}
#endif

#ifdef BUTTONS
  // M+ und H+ im STD_MODE_BLANK gedrueckt?
  if ((mode == STD_MODE_BLANK) && extModeDoubleButton.pressed()) {
    doubleExtModePressed();
  }
  
  // Taste Minuten++ (brighness++) gedrueckt?
  if (plusButton.pressed()) {
    buttonPlusPressed();
  }

  // Taste Stunden++ (brightness--) gedrueckt?
  if (minusButton.pressed()) {
    buttonMinusPressed();
  }

  // Taste Moduswechsel gedrueckt?
  if (modeButton.pressed()) {
    modePressed();
  }
#endif

	// Wenn noetig, den Screenbuffer neu erstellen und in die LEDs schreiben
	if (ScreenBufferNeedsUpdate) {
		ScreenBufferNeedsUpdate = false;
		switch (mode) {
		case STD_MODE_NORMAL:
			renderer.clearScreenBuffer(matrix);
			renderer.setTime(hour(), minute(), settings.getLanguage(), matrix);
			renderer.setCorners(minute(), matrix);
			if (!settings.getEsIst() && ((minute() / 5) % 6)) renderer.clearEntryWords(settings.getLanguage(), matrix);
			if (settings.getAlarm1() || settings.getAlarm2()) renderer.activateAlarmLed(matrix);
			break;
		case STD_MODE_AMPM:
			renderer.clearScreenBuffer(matrix);
			if (isAM()) renderer.setSmallText("AM", Renderer::TEXT_POS_MIDDLE, matrix);
			else renderer.setSmallText("PM", Renderer::TEXT_POS_MIDDLE, matrix);
			break;
		case STD_MODE_SECONDS:
			renderer.clearScreenBuffer(matrix);
			renderer.setCorners(minute(), matrix);
			for (uint8_t i = 0; i < 7; i++) {
				matrix[1 + i] |= (zahlenGross[second() / 10][i]) << 11;
				matrix[1 + i] |= (zahlenGross[second() % 10][i]) << 5;
			}
			break;
		case STD_MODE_DATE:
			renderer.clearScreenBuffer(matrix);
			if (day() < 10) renderer.setSmallText(("0" + String(day())), Renderer::TEXT_POS_TOP, matrix);
			else renderer.setSmallText(String(day()), Renderer::TEXT_POS_TOP, matrix);
			if (month() < 10) renderer.setSmallText(("0" + String(month())), Renderer::TEXT_POS_BOTTOM, matrix);
			else renderer.setSmallText(String(month()), Renderer::TEXT_POS_BOTTOM, matrix);
			renderer.setPixelInScreenBuffer(5, 4, matrix);
			renderer.setPixelInScreenBuffer(5, 9, matrix);
			break;
#ifdef RTC_BACKUP
		case STD_MODE_TEMP:
			renderer.clearScreenBuffer(matrix);
			for (uint8_t i = 0; i < 7; i++) {
				matrix[1 + i] |= (zahlenGross[(Rtc.GetTemperature().AsWholeDegrees() + RTC_TEMP_OFFSET) / 10][i]) << 11;
				matrix[1 + i] |= (zahlenGross[(Rtc.GetTemperature().AsWholeDegrees() + RTC_TEMP_OFFSET) % 10][i]) << 5;
			}
			renderer.setPixelInScreenBuffer(4, 0, matrix); // LED rechts oben setzen als "Grad"
			DEBUG_PRINTLN(String(Rtc.GetTemperature().AsFloat() + RTC_TEMP_OFFSET));
			break;
#endif
		case STD_MODE_SET_TIMER:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TR", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else renderer.setSmallText(String(timerMinutes), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case STD_MODE_TIMER:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TR", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText(String((timer - now() + 60) / 60), Renderer::TEXT_POS_BOTTOM, matrix);
			DEBUG_PRINTLN(String(timer - now()));
			break;
		case STD_MODE_ALARM_1:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("A1", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else {
				if (settings.getAlarm1()) {
					renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
					renderer.activateAlarmLed(matrix);
				}
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case STD_MODE_SET_ALARM_1:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getAlarmTime1()), minute(settings.getAlarmTime1()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getAlarmTime1()), settings.getLanguage(), matrix);
				renderer.activateAlarmLed(matrix);
			}
			break;
		case STD_MODE_ALARM_2:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("A2", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else {
				if (settings.getAlarm2()) {
					renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
					renderer.activateAlarmLed(matrix);
				}
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case STD_MODE_SET_ALARM_2:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getAlarmTime2()), minute(settings.getAlarmTime2()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getAlarmTime2()), settings.getLanguage(), matrix);
				renderer.activateAlarmLed(matrix);
			}
			break;
		case EXT_MODE_TEXT_MAIN:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("MA", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("IN", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
#ifdef LDR
		case EXT_MODE_LDR:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("LD", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else {
				if (settings.getUseLdr()) renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
#endif
		case EXT_MODE_BRIGHTNESS:
			renderer.clearScreenBuffer(matrix);
			for (uint8_t xb = 0; xb < map(settings.getBrightness(), 0, 255, 1, 10); xb++) {
				for (uint8_t yb = 0; yb <= xb; yb++) matrix[9 - yb] |= 1 << (14 - xb);
			}
			break;
		case EXT_MODE_COLOR:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("CO", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else renderer.setSmallText(String(settings.getColor()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TIMEOUT:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("FB", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else renderer.setSmallText(String(settings.getTimeout()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_LANGUAGE:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				if (sLanguage[settings.getLanguage()].length() == 2) renderer.setSmallText(sLanguage[settings.getLanguage()], Renderer::TEXT_POS_MIDDLE, matrix);
				else {
					renderer.setSmallText(sLanguage[settings.getLanguage()].substring(0, 2), Renderer::TEXT_POS_TOP, matrix);
					renderer.setSmallText(sLanguage[settings.getLanguage()].substring(2, 4), Renderer::TEXT_POS_BOTTOM, matrix);
				}
			}
			break;
		case EXT_MODE_TEXT_TIME:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TI", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("ME", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TIMESET:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(), minute(), settings.getLanguage(), matrix);
				renderer.setCorners(minute(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(), settings.getLanguage(), matrix);
			}
			break;
		case EXT_MODE_IT_IS:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("IT", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else {
				if (settings.getEsIst()) renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_DAYSET:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("DD", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else renderer.setSmallText(String(day()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_MONTHSET:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("MM", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else renderer.setSmallText(String(month()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_YEARSET:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("YY", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i < 10; i++) matrix[i] = 0;
			else renderer.setSmallText(String(year()).substring(2, 4), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TEXT_NIGHTOFF:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("NT", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("OF", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_NIGHTOFF:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getNightOffTime()), minute(settings.getNightOffTime()), settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getNightOffTime()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
			}
			break;
		case EXT_MODE_TEXT_NIGHTON:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("NT", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("ON", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_NIGHTON:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getNightOnTime()), minute(settings.getNightOnTime()), settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getNightOnTime()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
			}
			break;
		case EXT_MODE_TEXT_TEST:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TE", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("ST", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TEST:
			renderer.clearScreenBuffer(matrix);
			if (testColumn == 10) testColumn = 0;
			matrix[testColumn] = 0b1111111111110000;
			testColumn++;
			break;
		case STD_MODE_BLANK:
			renderer.clearScreenBuffer(matrix);
			break;
		default:
			break;
		}
#if defined(IR_LETTER_OFF)
		// Die LED hinter dem IR-Sensor abschalten.
		renderer.unsetPixelInScreenBuffer(8, 9, matrix);
#endif
		// Screenbuffer in die LEDs schreiben
		ledDriver.writeScreenBufferToLEDs(matrix, settings.getColor());
#ifdef DEBUG_MATRIX
		// Screenbuffer auf der Konsole ausgeben
		debug.debugScreenBuffer(matrix);
#endif
	}
}

/******************************************************************************
"M+" & "H+" gedrueckt im BLANK MODE
******************************************************************************/
void doubleExtModePressed() {
  DEBUG_PRINTLN(F("Minutes plus AND hours plus pressed in STD_MODE_BLANK..."));
  DEBUG_FLUSH();
  unsigned long initialMillis = millis();
  while (minusButton.pressedRaw() && plusButton.pressedRaw()) {
  }
  setMode(EXT_MODE_START);
  DEBUG_PRINT(F("Entering EXT_MODEs, mode is now "));
  DEBUG_PRINT(mode);
  DEBUG_PRINTLN(F("..."));
  DEBUG_FLUSH();
}

/******************************************************************************
"Mode" gedrueckt
******************************************************************************/

void modePressed() {

	// Alarm abschalten
	if (alarmOn) {
		alarmOn = false;
		digitalWrite(PIN_BUZZER, LOW);
		return;
	}

	// Nachtmode abschalten
	if (mode == STD_MODE_BLANK) {
		setMode(STD_MODE_NORMAL);
		return;
	}

	// Mode weiterschalten und ggf. Fallback setzen
	setMode(mode++);
	switch (mode) {
	case STD_MODE_COUNT:
	case EXT_MODE_COUNT:
		setMode(STD_MODE_NORMAL);
		break;
	case STD_MODE_TIMER:
		if (!timerSet) setMode(mode++);
		break;
	case STD_MODE_SET_ALARM_1:
		if (!settings.getAlarm1()) setMode(mode++);;
		break;
	case STD_MODE_SET_ALARM_2:
		if (!settings.getAlarm2()) setMode(mode++);;
		break;
#ifdef LDR
	case EXT_MODE_BRIGHTNESS:
		if (settings.getUseLdr()) setMode(mode++);
		break;
#endif
	case EXT_MODE_TEST:
		testColumn = 0;
		return;
	case STD_MODE_AMPM:
	case STD_MODE_SECONDS:
	case STD_MODE_DATE:
#ifdef RTC_BACKUP
	case STD_MODE_TEMP:
#endif
		fallBackCounter = settings.getTimeout();
		break;
	default:
		fallBackCounter = 0;
		break;
	}

	// Einstellungen sichern
	settings.saveToEEPROM();

#ifdef RTC_BACKUP
	// RTC stellen
	Rtc.SetDateTime(now());
#endif
}

/******************************************************************************
"+" gedrueckt
******************************************************************************/

void buttonPlusPressed() {
	DEBUG_PRINTLN("+ pressed.");
	ScreenBufferNeedsUpdate = true;
	switch (mode) {
	case STD_MODE_SET_TIMER:
		if (timerMinutes < 100) timerMinutes++;
		timer = now() + timerMinutes * 60;
		timerSet = true;
		break;
	case STD_MODE_ALARM_1:
		settings.toggleAlarm1();
		break;
	case STD_MODE_SET_ALARM_1:
		settings.setAlarmTime1(settings.getAlarmTime1() + 3600);
#ifdef DEBUG
		debug.debugTime("Alarm 1:", settings.getAlarmTime1());
#endif
		break;
	case STD_MODE_ALARM_2:
		settings.toggleAlarm2();
		break;
	case STD_MODE_SET_ALARM_2:
		settings.setAlarmTime2(settings.getAlarmTime2() + 3600);
#ifdef DEBUG
		debug.debugTime("Alarm 2:", settings.getAlarmTime2());
#endif
		break;
	case EXT_MODE_TEXT_MAIN:
		setMode(EXT_MODE_TEXT_TIME);
		break;
#ifdef LDR
	case EXT_MODE_LDR:
		settings.toggleUseLdr();
		break;
#endif
	case EXT_MODE_BRIGHTNESS:
		settings.setBrightness(constrain(settings.getBrightness() + 10, 0, 255));
		DEBUG_PRINTLN(settings.getBrightness());
		break;
	case EXT_MODE_COLOR:
		if (settings.getColor() < maxColor) settings.setColor(settings.getColor() + 1);
		else settings.setColor(0);
		break;
	case EXT_MODE_TIMEOUT:
		if (settings.getTimeout() < 99) settings.setTimeout(settings.getTimeout() + 1);
		break;
	case EXT_MODE_LANGUAGE:
		if (settings.getLanguage() < LANGUAGE_COUNT - 1) settings.setLanguage(settings.getLanguage() + 1);
		else settings.setLanguage(0);
		break;
	case EXT_MODE_TEXT_TIME:
		setMode(EXT_MODE_TEXT_TEST);
		break;
	case EXT_MODE_TIMESET:
		setTime(hour() + 1, minute(), second(), day(), month(), year());
#ifdef DEBUG
		debug.debugTime("Time set:", now());
#endif
		break;
	case EXT_MODE_IT_IS:
		settings.toggleEsIst();
		break;
	case EXT_MODE_DAYSET:
		setTime(hour(), minute(), second(), day() + 1, month(), year());
		break;
	case EXT_MODE_MONTHSET:
		setTime(hour(), minute(), second(), day(), month() + 1, year());
		break;
	case EXT_MODE_YEARSET:
		setTime(hour(), minute(), second(), day(), month(), year() + 1);
		break;
	case EXT_MODE_NIGHTOFF:
		settings.setNightOffTime(settings.getNightOffTime() + 3600);
#ifdef DEBUG
		debug.debugTime("Night Off:", settings.getNightOffTime());
#endif
		break;
	case EXT_MODE_NIGHTON:
		settings.setNightOnTime(settings.getNightOnTime() + 3600);
#ifdef DEBUG
		debug.debugTime("Night On:", settings.getNightOnTime());
#endif
		break;
	case EXT_MODE_TEXT_TEST:
		setMode(EXT_MODE_TEXT_MAIN);
		break;
	default:
		break;
	}
}

/******************************************************************************
"-" gedrueckt
******************************************************************************/

void buttonMinusPressed() {
	DEBUG_PRINTLN("- pressed.");
	ScreenBufferNeedsUpdate = true;
	switch (mode) {
	case STD_MODE_SET_TIMER:
		if (timerMinutes > 0) {
			timerMinutes--;
			if (timerMinutes == 0) timerSet = false;
			else {
				timer = now() + timerMinutes * 60;
				timerSet = true;
			}
		}
		break;
	case STD_MODE_ALARM_1:
		settings.toggleAlarm1();
		break;
	case STD_MODE_SET_ALARM_1:
		settings.setAlarmTime1(settings.getAlarmTime1() + 300);
#ifdef DEBUG
		debug.debugTime("Alarm 1:", settings.getAlarmTime1());
#endif
		break;
	case STD_MODE_ALARM_2:
		settings.toggleAlarm2();
		break;
	case STD_MODE_SET_ALARM_2:
		settings.setAlarmTime2(settings.getAlarmTime2() + 300);
#ifdef DEBUG
		debug.debugTime("Alarm 2:", settings.getAlarmTime2());
#endif
		break;
	case EXT_MODE_TEXT_MAIN:
		setMode(EXT_MODE_TEXT_TEST);
		break;
#ifdef LDR
	case EXT_MODE_LDR:
		settings.toggleUseLdr();
		break;
#endif
	case EXT_MODE_BRIGHTNESS:
		settings.setBrightness(constrain(settings.getBrightness() - 10, 0, 255));
		DEBUG_PRINTLN(settings.getBrightness());
		break;
	case EXT_MODE_COLOR:
		if (settings.getColor() > 0) settings.setColor(settings.getColor() - 1);
		else settings.setColor(maxColor);
		break;
	case EXT_MODE_TIMEOUT:
		if (settings.getTimeout() > 0) settings.setTimeout(settings.getTimeout() - 1);
		break;
	case EXT_MODE_LANGUAGE:
		if (settings.getLanguage() > 0) settings.setLanguage(settings.getLanguage() - 1);
		else settings.setLanguage(LANGUAGE_COUNT - 1);
		break;
	case EXT_MODE_TEXT_TIME:
		setMode(EXT_MODE_TEXT_MAIN);
		break;
	case EXT_MODE_TIMESET:
		setTime(hour(), minute() + 1, 0, day(), month(), year());
#ifdef DEBUG
		debug.debugTime("Time set:", now());
#endif
		break;
	case EXT_MODE_IT_IS:
		settings.toggleEsIst();
		break;
	case EXT_MODE_DAYSET:
		setTime(hour(), minute(), second(), day() - 1, month(), year());
		break;
	case EXT_MODE_MONTHSET:
		setTime(hour(), minute(), second(), day(), month() - 1, year());
		break;
	case EXT_MODE_YEARSET:
		setTime(hour(), minute(), second(), day(), month(), year() - 1);
		break;
	case EXT_MODE_NIGHTOFF:
		settings.setNightOffTime(settings.getNightOffTime() + 300);
#ifdef DEBUG
		debug.debugTime("Night Off:", settings.getNightOffTime());
#endif
		break;
	case EXT_MODE_NIGHTON:
		settings.setNightOnTime(settings.getNightOnTime() + 300);
#ifdef DEBUG
		debug.debugTime("Night On:", settings.getNightOnTime());
#endif
		break;
	case EXT_MODE_TEXT_TEST:
		setMode(EXT_MODE_TEXT_TIME);
		break;
	default:
		break;
	}
}

/******************************************************************************
IR empfangen
******************************************************************************/

#ifdef IR_REMOTE
void remoteAction(uint32_t irDecodeResults) {
	switch (irDecodeResults) {
	case IR_CODE_ONOFF:
		setDisplayToToggle();
		break;
	case IR_CODE_TIME:
		settings.saveToEEPROM();
#ifdef RTC_BACKUP
		Rtc.SetDateTime(now());
#endif
		if (alarmOn) {
			alarmOn = false;
			digitalWrite(PIN_BUZZER, LOW);
		}
		setMode(STD_MODE_NORMAL);
		break;
	case IR_CODE_MODE:
		modePressed();
		break;
	case IR_CODE_EXTMODE:
		if (mode < EXT_MODE_START) setMode(EXT_MODE_START);
		else modePressed();
		break;
	case IR_CODE_PLUS:
		buttonPlusPressed();
		break;
	case IR_CODE_MINUS:
		buttonMinusPressed();
		break;
	default:
		break;
	}
}
#endif

/******************************************************************************
Sonstiges
******************************************************************************/

// Mode setzen
void setMode(Mode newmode) {
	DEBUG_PRINT("Mode: ");
	DEBUG_PRINTLN(newmode);
	ScreenBufferNeedsUpdate = true;
	lastMode = mode;
	mode = newmode;
}

// LEDs ausschalten
void setLedsOff() {
	DEBUG_PRINTLN("LEDs off.");
	setMode(STD_MODE_BLANK);
}

// LEDs einschalten
void setLedsOn() {
	DEBUG_PRINTLN("LEDs on.");
	setMode(lastMode);
}

// LEDs toggeln
void setDisplayToToggle() {
	if (mode != STD_MODE_BLANK) setLedsOff();
	else setLedsOn();
}

#ifdef RTC_BACKUP
// Zeit von RTC holen
time_t getRtcTime() {
	DEBUG_PRINTLN("*** ESP set from RTC. ***");
#ifdef SYSLOG_SERVER
	syslog.log(LOG_DEBUG, "ESP set from RTC.");
#endif
	return Rtc.GetDateTime();
}
#endif

// Zeit mit NTP holen
time_t getNtpTime() {
	if (WiFi.status() == WL_CONNECTED) {
		while (Udp.parsePacket() > 0);
		WiFi.hostByName(NTP_SERVER, timeServerIP);
		sendNTPpacket(timeServerIP);
		uint32_t beginWait = millis();
		while (millis() - beginWait < 1500) {
			int size = Udp.parsePacket();
			if (size >= 48) {
				Udp.read(packetBuffer, 48);
				unsigned long secsSince1900;
				secsSince1900 = (unsigned long)packetBuffer[40] << 24;
				secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
				secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
				secsSince1900 |= (unsigned long)packetBuffer[43];
#ifdef RTC_BACKUP
				DEBUG_PRINTLN("*** RTC set from NTP. ***");
#ifdef SYSLOG_SERVER
				syslog.log(LOG_DEBUG, "RTC set from NTP.");
#endif
				Rtc.SetDateTime(timeZone.toLocal(secsSince1900 - 2208988800UL));
#endif
				DEBUG_PRINTLN("*** ESP set from NTP. ***");
#ifdef SYSLOG_SERVER
				syslog.log(LOG_DEBUG, "ESP set from NTP.");
#endif
				return (timeZone.toLocal(secsSince1900 - 2208988800UL));
			}
		}
		DEBUG_PRINTLN("No NTP Response. :(");
#ifdef SYSLOG_SERVER
		syslog.log(LOG_ERR, "No NTP Response.");
#endif
#ifdef RTC_BACKUP
		return getRtcTime();
#else
		return now();
#endif
	}
	DEBUG_PRINTLN("Wifi not connected. :(");
	return now();
	}

// NTP Paket senden
void sendNTPpacket(IPAddress & address) {
	memset(packetBuffer, 0, 48);
	packetBuffer[0] = 0b11100011;
	packetBuffer[1] = 0;
	packetBuffer[2] = 6;
	packetBuffer[3] = 0xEC;
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	Udp.beginPacket(address, 123);
	Udp.write(packetBuffer, 48);
	Udp.endPacket();
}

/******************************************************************************
Webserver
******************************************************************************/

// Setup
void setupWebServer() {
	server.onNotFound(handleNotFound);
	server.on("/", handleRoot);
	server.on("/handle_TOGGLEBLANK", handle_TOGGLEBLANK);
	server.on("/handle_BUTTON_TIME", handle_BUTTON_TIME);
	server.on("/handle_BUTTON_MODE", handle_BUTTON_MODE);
	server.on("/handle_BUTTON_EXTMODE", handle_BUTTON_EXTMODE);
	server.on("/handle_BUTTON_PLUS", handle_BUTTON_PLUS);
	server.on("/handle_BUTTON_MINUS", handle_BUTTON_MINUS);
	server.begin();
}

// Seite 404
void handleNotFound() {
	server.send(404, "text/plain", "404 - File Not Found.");
}

// Seite /
void handleRoot() {
	String message = "<!doctype html>";
	message += "<html>";
	message += "<head>";
	message += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
	message += "<title>";
	message += HOSTNAME;
	message += "</title>";
	message += "<style>";
	message += "body {background-color:#FFFFFF; text-align:center; font-family:verdana; color:#333333;}";
	message += "button {background-color:#1FA3EC; border: 5px solid #FFFFFF; color:#FFFFFF; width: 200px; padding:15px 32px; text-align:center; display:inline-block; font-size:16px;}";
	message += "</style>";
	message += "</head>";
	message += "<body>";
	message += "<h1>";
	message += HOSTNAME;
	message += "</h1>";
	message += "<button onclick=\"window.location.href='/handle_TOGGLEBLANK'\">On/Off</button>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_TIME'\">Time</button>";
	message += "<br><br>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_MODE'\">Mode</button>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_EXTMODE'\">Ext. Mode</button>";
	message += "<br><br>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_PLUS'\">+</button>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_MINUS'\">-</button>";
	message += "<br><br>";
#ifdef RTC_BACKUP
	message += "Temperature: " + String(Rtc.GetTemperature().AsFloat() + RTC_TEMP_OFFSET) + "&#176;C / " + String((Rtc.GetTemperature().AsFloat() + RTC_TEMP_OFFSET) * 1.8 + 32) + "&#176;F";
	message += "<br>";
#endif
	message += "<font size=2>";
	message += "Firmware: ";
	message += FIRMWARE_VERSION;
	message += "</font>";
	message += "</body>";
	message += "</html>";
	server.send(200, "text/html", message);
}

void handle_TOGGLEBLANK() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	server.send(200, "text/html", message);
	setDisplayToToggle();
}

void handle_BUTTON_TIME() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	server.send(200, "text/html", message);
	// Alarm abschalten
	if (alarmOn) {
		alarmOn = false;
		digitalWrite(PIN_BUZZER, LOW);
	}
	settings.saveToEEPROM();
#ifdef RTC_BACKUP
	Rtc.SetDateTime(now());
#endif
	setMode(STD_MODE_NORMAL);
}

void handle_BUTTON_MODE() {
	String message = "<!doctype html><html><head><script>window.onload = function() {window.location.replace('/')};</script></head><body></body></html>";
	server.send(200, "text/html", message);
	modePressed();
}

void handle_BUTTON_EXTMODE() {
	String message = "<!doctype html><html><head><script>window.onload = function() {window.location.replace('/')};</script></head><body></body></html>";
	server.send(200, "text/html", message);
	if (mode < EXT_MODE_START) setMode(EXT_MODE_START);
	else modePressed();
}

void handle_BUTTON_PLUS() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	server.send(200, "text/html", message);
	buttonPlusPressed();
}

void handle_BUTTON_MINUS() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	server.send(200, "text/html", message);
	buttonMinusPressed();
}
