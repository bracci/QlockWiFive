/******************************************************************************
   Configuration.h

   D0 = NodeMCU_LED
   D1 = PIN_WIRE_SCL
   D2 = PIN_WIRE_SDA
   D3 = PIN_IR_RECEIVER
   D4 = ESP8266_LED
   D5 = PIN_BUZZER
   D6 = nc
   D7 = PIN_LEDS_CLOCK
   D8 = PIN_LEDS_DATA
   A0 = PIN_LDR

******************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#define CONFIG_WIFIVE
#define CONFIG_WIFIVE_MINI

#define USE_EVENTS

/******************************************************************************
 * 
 *****************************************************************************/
#ifdef CONFIG_WIFIVE

  #define HOSTNAME "QlockWiFive"
  #define OTA_PASS "1234"
  #define NTP_SERVER "pool.ntp.org"
  
  //#define SYSLOG_SERVER "192.168.0.1"
  //#define SYSLOG_FACILITY LOG_LOCAL0
  //#define SYSLOG_PORT 514
  
  #define RTC_BACKUP
  #define RTC_TEMP_OFFSET 0
  
  #define BOARD_LED
  
  //#define NONE_TECHNICAL_ZERO
  
  #define LDR
  
  #define MIN_BRIGHTNESS 10
  #define MAX_BRIGHTNESS 255
  
  //#define TIMEZONE_PST
  //#define TIMEZONE_MST
  //#define TIMEZONE_CST
  //#define TIMEZONE_EST
  //#define TIMEZONE_GMT
  #define TIMEZONE_CET
  //#define TIMEZONE_AEST
  
  //#define IR_LETTER_OFF
  
  #define IR_REMOTE
  
  // HX1838 Remote
  #define IR_CODE_ONOFF   16769565
  #define IR_CODE_TIME    16753245
  #define IR_CODE_MODE    16736925
  #define IR_CODE_EXTMODE 16748655
  #define IR_CODE_PLUS    16754775
  #define IR_CODE_MINUS   16769055
  
//  #define LED_LAYOUT_HORIZONTAL
  #define LED_LAYOUT_VERTICAL
  
  #define LED_RGB
//  #define LED_RGBW
//  #define MATRIX_XXL
  
//  #define LED_DRIVER_APA102
//  #define LED_DRIVER_APA104
//  #define LED_DRIVER_APA106
//  #define LED_DRIVER_DOTSTAR
//  #define LED_DRIVER_GW6205
//  #define LED_DRIVER_GW6205_400
//  #define LED_DRIVER_LPD1886
//  #define LED_DRIVER_LPD1886_8BIT
//  #define LED_DRIVER_LPD8806
  #define LED_DRIVER_NEOPIXEL
//  #define LED_DRIVER_P9813
//  #define LED_DRIVER_PL9823
//  #define LED_DRIVER_SK6812
//  #define LED_DRIVER_SK6822
//  #define LED_DRIVER_SK9822
//  #define LED_DRIVER_SM16716
//  #define LED_DRIVER_TM1803
//  #define LED_DRIVER_TM1804
//  #define LED_DRIVER_TM1809
//  #define LED_DRIVER_TM1812
//  #define LED_DRIVER_TM1829
//  #define LED_DRIVER_UCS1903
//  #define LED_DRIVER_UCS1903B
//  #define LED_DRIVER_UCS1904
//  #define LED_DRIVER_UCS2903
//  #define LED_DRIVER_WS2801
//  #define LED_DRIVER_WS2803
//  #define LED_DRIVER_WS2811
//  #define LED_DRIVER_WS2811_400
//  #define LED_DRIVER_WS2812
//  #define LED_DRIVER_WS2812B
//  #define LED_DRIVER_WS2813
//  #define LED_DRIVER_WS2852
  
  #define PIN_IR_RECEIVER D4
  #define PIN_LED         D6
  #define PIN_BUZZER      D8
  #define PIN_LEDS_CLOCK  D5
  #define PIN_LEDS_DATA   D7
  #define PIN_LDR         A0
  
  #define SERIAL_SPEED 115200
  //#define DEBUG
  //#define DEBUG_MATRIX
  //#define DEBUG_FPS

#endif // CONFIG_WIFIVE

#ifdef CONFIG_WIFIVE_MINI

  #define HOSTNAME "QlockWiFiveMini"
  #define OTA_PASS "1234"
  #define NTP_SERVER "pool.ntp.org"
  //#define RTC_BACKUP
  //#define RTC_TEMP_OFFSET -5
  #define LDR
  #define MIN_BRIGHTNESS 5
  #define MAX_BRIGHTNESS 255
  #define TIMEZONE_CET
  #define IR_LETTER_OFF
  #define IR_REMOTE
  #define IR_CODE_ONOFF   16769055
  #define IR_CODE_TIME    16752735
  #define IR_CODE_MODE    16720095
  #define IR_CODE_EXTMODE 16736415
  #define IR_CODE_PLUS    16734375
  #define IR_CODE_MINUS   16730295
  #define LED_LAYOUT_VERTICAL
  #define LED_RGB
  #define LED_DRIVER_NEOPIXEL
  //#define BOARD_LED
  #define PIN_IR_RECEIVER D6
  //#define PIN_LED         D4
  #define PIN_BUZZER      D8
  #define PIN_LEDS_DATA   D7
  #define PIN_LDR         A0
  #define BUTTONS
  /*
   * Die Zeit in Millisekunden, innerhalb derer Prellungen der Taster nicht als Druecken zaehlen.
   * (Und damit auch die Tastaturwiederholrate)
   * Default: 300
   */
  #define BUTTONS_PRESSING_AGAINST LOW
  #define PIN_M_PLUS D0
  #define PIN_H_PLUS D5
  #define PIN_MODE D4
  #define SERIAL_SPEED 115200

#endif // CONFIG_WIFIVE_MINI

#endif
