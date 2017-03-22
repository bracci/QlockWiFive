# QLOCKWORK
### Eine Firmware der Selbstbau-QLOCKTWO.

### Features:
```
Almost no electronics needed.
Only ESP8266 (NodeMCU or WeMos D1 mini) and an LED-stripe.
Optional support for RTC, LDR, Buzzer and IR-remote.
Supports more than 30 types of LED stripes.
RGB and RGBW (experimental).
Horizontal and vertial LED layout.
16 Languages.
Timer.
Alarm.
NTP timesync.
RTC timesync as backup.
Timezones.
DST.
Webserver to controll the clock.
OTA.
WiFi manager for initial setup via accesspoint.
(RGBW ist only tested with LPD8806 of the CLT2.)
```
### Standard modes
```
Time.
AM/PM.
Seconds.
Date.
Temperature (with RTC only).
Timer.
Alarm 1.
Alarm 2.
```
### Extended modes
```
Brightnesscontrol.
Color.
Fallbacktime.
Language.
Set Time.
"It is" on/off.
Set Date.
Nightmode on/off.
Testmode.
```
### Needed libraries:
```
via Arduino:
WiFiManager by tzapu
DNSServer by Kristijan Novoselic
ArduinoOTA by Ivan Grokhotkov
ESP8266WebServer by Ivan Grokhotkov
ESP8266WiFi by Ivan Grokhotkov
FastLED by Daniel Garcia
RTC by Makuna
IRremoteESP8266 by Sebastien Warin

via Web:
https://github.com/PaulStoffregen/Time
https://github.com/JChristensen/Timezone

There is a warning from FastLED about SPI when compiling. Just ignore it.
```

# QLOCKWORK
### Eine Firmware der Selbstbau-QLOCKTWO.

Sie gleicht die Zeit jede Stunde per NTP mit einem Zeitserver im Internet ab. Auf der Web-Seite kann man die Uhr steuern.
Updates sind OTA moeglich. Dazu im Arduino IDE den ESP als Port auswaehlen.

WiFi Manager: Wenn die Uhr sich beim Start mit keinem WLAN verbinden kann, schaltet sie einen AccessPoint ein. Dann ein Handy oder Tablet mit diesem verbinden und die WLAN Daten eingeben.

Die Firmware gibt es hier: https://github.com/ch570512/Qlockwork

### Standard Modi
```
Zeitanzeige: Der Standardmodus der Uhr. Er zeigt die Zeit an. :)
Anzeige AM/PM: Zeigt an, ob es vormittags (AM) oder nachmittags (PM) ist.
Sekunden: Anzeige der Sekunden.
Datum: Anzeige des aktuellen Tages und Monats.
Temperatur: Anzeige der gemessenen Temperatur.
Timer Set: Setzt den Minuten-Timer. Null schaltet den Timer ab.
           Der Timer startet sofort mit dem Druck auf + oder -.
Timer: Anzeige der Restzeit wenn ein Timer gesetzt ist.
Alarm1 ein/aus
Alarm1: Setzt den ersten 24-Stunden-Alarm wenn Alarm1 "ein" ist.
Alarm2 ein/aus
Alarm2: Setzt den zweiten 24-Stunden-Alarm wenn Alarm2 "ein" ist.
```
### Erweiterte Modi
```
Titel MAIN: H+ und M+ druecken um direkt in die naechste oder vorhergehende Kategorie zu wechseln.
Automatische Helligkeitsregelung ein/aus
Helligkeitsregelung
Farbe
Ruecksprungverzoegerung (FB nn): Wie lange soll es dauern, bis z.B. aus der Sekundenanzeige wieder zurueck in die
                                 Zeitanzeige gewechselt wird. (0 = deaktiviert.)
Sprache (DE/CH/EN/...): Die passende Sprache zur benutzten Front waehlen.

Titel TIME: H+ und M+ druecken um direkt in die naechste oder vorhergehende Kategorie zu wechseln.
Zeit einstellen: H+ und M+ druecken um die Zeit zu stellen. Die Sekunden springen mit jedem Druck auf Null.
"Es ist" anzeigen oder nicht (IT EN/DA)
Tag einstellen   (DD nn): H+ und M+ druecken um den aktuellen Tag einzustellen.
Monat einstellen (MM nn): H+ und M+ druecken um den aktuellen Monat einzustellen.
Jahr einstellen  (YY nn): H+ und M+ druecken um das aktuelle Jahr einzustellen.
Nachtauschaltung        (NI OF): H+ und M+ druecken um die Ausschaltzeit des Displays einzustellen.
Nachtwiedereinschaltung (NI ON): H+ und M+ druecken um die Einschaltzeit des Displays einzustellen.

Titel TEST: H+ und M+ druecken um direkt in die naechste oder vorhergehende Kategorie zu wechseln.
LED-Test: Laesst einen waagerechten Streifen ueber das Display wandern.
```
### Configuration.h
```
#define HOSTNAME            Der Name der Uhr.
#define OTA_PASS            Kennwort fuer "Over the Air" Updates.
#define NTP_SERVER          Abzufragender NTP-Server.
#define RTC_BACKUP          Eine RTC als Backup verwenden.
#define RTC_TEMP_OFFSET     Gibt an, um wieviel Grad die gemessene Temperatur (+ oder -) korrigiert werden soll.
                            Nur ganze Zahlen verwenden.
#define BOARD_LED           Zeigt mit Hilfe der LED auf dem Board die Funktion der RTC an. Sie blinkt einmal
                            pro Sekunde.
#define NONE_TECHNICAL_ZERO Zeigt die Null ohne den diagonalen Strich.


Die Zeitzone in der sich die Uhr befindet. Wichtig fuer den GMT-Versatz und die Sommer-/Winterzeitumstellung.

#define TIMEZONE_PST
#define TIMEZONE_MST
#define TIMEZONE_CST
#define TIMEZONE_EST
#define TIMEZONE_GMT
#define TIMEZONE_CET
#define TIMEZONE_AEST


#define LDR                 Einen LDR fuer die Helligkeitsregelung verwenden.


#define MIN_BRIGHTNESS 10   Minimale Helligkeit der LEDs im Bereich von 0 bis 255.
#define MAX_BRIGHTNESS 255  Maxmale Helligkeit der LEDs im Bereich von 0 bis 255.


#define LED_LAYOUT_HORIZONTAL  Waagerecht und Eck-LEDs am Ende des Stripes. (Von vorne gesehen.)

111                 114                 112
000 001 002 003 004 005 006 007 008 009 010
021 020 019 018 017 016 015 014 013 012 011
022 023 024 025 026 027 028 029 030 031 032
043 042 041 040 039 038 037 036 035 034 033
044 045 046 047 048 049 050 051 052 053 054
065 064 063 062 061 060 059 058 057 056 055
066 067 068 069 070 071 072 073 074 075 076
087 086 085 084 083 082 081 080 079 078 077
088 089 090 091 092 093 094 095 096 097 098
109 108 107 106 105 104 103 102 101 100 099
110                                     113

#define LED_LAYOUT_VERTICAL  Senkrecht und Eck-LEDs innerhalb des Stripes. (Von vorne gesehen.)

000                 114                 102
001 021 022 041 042 061 062 081 082 101 103
002 020 023 040 043 060 063 080 083 100 104
003 019 024 039 044 059 064 079 084 099 105
004 018 025 038 045 058 065 078 085 098 106
005 017 026 037 046 057 066 077 086 097 107
006 016 027 036 047 056 067 076 087 096 108
007 015 028 035 048 055 068 075 088 095 109
008 014 029 034 049 054 069 074 089 094 110
009 013 030 033 050 053 070 073 090 093 111
010 012 031 032 051 052 071 072 091 092 112
011                                     113


#define LED_RGB
#define LED_RGBW  Da RGBW von FAST-LED (noch) nicht unterstuetzt wird, ist dies ein Hack welcher nur mit dem
                  LPD8806 Treiber und dem Streifen der CLT2 getestet ist. Es ist zu vermuten, dass andere
                  Streifen eine abweichende Ansteuerung verwenden und dehalb nicht korrekt funktionieren.


Alle von FAST-LED unterstützten LED-Treiber koennen verwendet werden:
APA102, APA104, APA106, DOTSTAR, GW6205, GW6205_400, LPD1886, LPD1886_8BIT, LPD8806, NEOPIXEL, 
P9813, PL9823, SK6812, SK6822, SK9822, SM16716, TM1803, TM1804, TM1809, TM1812, TM1829, UCS1903,
UCS1903B, UCS1904, UCS2903, WS2801, WS2803, WS2811, WS2811_400, WS2812, WS2812B, WS2813, WS2852.


#define IR_LETTER_OFF  Schaltet die LED hinter dem IR-Sensor dauerhaft ab. Das verbessert den IR-Empfang.
                       Hier das K vor Uhr.

#define IR_REMOTE  IR-Fernbedienung verwenden.

Jede Fernbedienung kann verwendet werden. Es werden 6 Tasten unterstützt.
Um die Fernbedienung anzulernen "#define DEBUG" einschalten und einen Knopf auf der Fernbedienung druecken.
Den in der seriellen Konsole angezeigten Code dann in die Datei "Configuration.h" schreiben.

#define IR_CODE_ONOFF   16769565
#define IR_CODE_TIME    16753245
#define IR_CODE_MODE    16736925
#define IR_CODE_EXTMODE 16748655
#define IR_CODE_PLUS    16754775
#define IR_CODE_MINUS   16769055


#define SERIAL_SPEED  Geschwindigkeit der seriellen Schnittstelle fuer die serielle Konsole.
#define DEBUG         Gibt technische Informationen in der seriellen Konsole aus.
#define DEBUG_MATRIX  Rendert die Ausgabe der Matrix fuer die deutsche Front in der seriellen Konsole.
#define DEBUG_FPS     Durchlaeufe der loop() pro Sekunde.
```
