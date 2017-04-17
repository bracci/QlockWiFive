/******************************************************************************
   LedDriver.cpp
******************************************************************************/

#include "LedDriver.h"

LedDriver::LedDriver() {
#ifdef LED_DRIVER_NEOPIXEL
	FastLED.addLeds<NEOPIXEL, PIN_LEDS_DATA>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_TM1803
	FastLED.addLeds<TM1803, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_TM1804
	FastLED.addLeds<TM1804, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_TM1809
	FastLED.addLeds<TM1809, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2811
	FastLED.addLeds<WS2811, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2812
	FastLED.addLeds<WS2812, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2812B
	FastLED.addLeds<WS2812B, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_APA104
	FastLED.addLeds<APA104, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_UCS1903
	FastLED.addLeds<UCS1903, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_UCS1903B
	FastLED.addLeds<UCS1903B, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_GW6205
	FastLED.addLeds<GW6205, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_GW6205_400
	FastLED.addLeds<GW6205_400, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2801
	FastLED.addLeds<WS2801, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_SM16716
	FastLED.addLeds<SM16716, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_LPD8806
	FastLED.addLeds<LPD8806, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_P9813
	FastLED.addLeds<P9813, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_APA102
	FastLED.addLeds<APA102, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_DOTSTAR
	FastLED.addLeds<DOTSTAR, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_SK9822
	FastLED.addLeds<SK9822, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_SK6822
	FastLED.addLeds<SK6822, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_SK6812
	FastLED.addLeds<SK6812, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_TM1829
	FastLED.addLeds<TM1829, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_UCS2903
	FastLED.addLeds<UCS2903, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_UCS1904
	FastLED.addLeds<UCS1904, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_PL9823
	FastLED.addLeds<PL9823, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_TM1812
	FastLED.addLeds<TM1812, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2852
	FastLED.addLeds<WS2852, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_APA106
	FastLED.addLeds<APA106, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2813
	FastLED.addLeds<WS2813, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2811_400
	FastLED.addLeds<WS2811_400, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_LPD1886
	FastLED.addLeds<LPD1886, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_LPD1886_8BIT
	FastLED.addLeds<LPD1886_8BIT, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_DMXSIMPLE
	FastLED.addLeds<DMXSIMPLE, PIN_LEDS_DATA, RGB>(leds, NUM_LEDS);
#endif
#ifdef LED_DRIVER_WS2803
	FastLED.addLeds<WS2803, PIN_LEDS_DATA, PIN_LEDS_CLOCK, RGB>(leds, NUM_LEDS);
#endif
}

void LedDriver::writeScreenBufferToLEDs(word matrix[16], uint8_t color) {
	FastLED.clear();
	for (uint8_t y = 0; y < 10; y++) {
		for (uint8_t x = 5; x < 16; x++) {
			word t = 1 << x;
			if ((matrix[y] & t) == t) setPixel(15 - x, y, color);
		}
	}
	byte cornerLedCount[] = { 1, 0, 3, 2, 4 };
	for (uint8_t i = 0; i < 5; i++) if ((matrix[cornerLedCount[i]] & 0b0000000000010000) > 0) setPixel(110 + i, color);
	FastLED.show();
}

void LedDriver::setPixel(uint8_t x, uint8_t y, uint8_t color) {
#ifdef LED_LAYOUT_HORIZONTAL
	setPixel(x + (y * 11), color);
#endif
#ifdef LED_LAYOUT_VERTICAL
	setPixel(y + (x * 10), color);
#endif
}

void LedDriver::setPixel(uint8_t num, uint8_t color) {

#ifdef LED_RGB
	uint8_t red = defaultColors[color].red;
	uint8_t green = defaultColors[color].green;
	uint8_t blue = defaultColors[color].blue;
	uint32_t ledColor_rgb = red * 0x010000 + green * 0x000100 + blue;
#endif

#ifdef LED_RGBW
	uint8_t red = defaultColors[color].red;
	uint8_t green = defaultColors[color].green;
	uint8_t blue = defaultColors[color].blue;
	uint8_t white = getWhite(red, green, blue);
	red -= white;
	green -= white;
	blue -= white;
	uint32_t ledColor_wbg = white * 0x010000 + blue * 0x000100 + green;
	uint32_t ledColor_r = red * 0x010000;
#endif

#ifdef LED_LAYOUT_HORIZONTAL
#ifdef LED_RGB
	if (num < 110) {
		if ((num / 11) % 2 == 0) leds[num] = ledColor_rgb;
		else leds[((num / 11) * 11) + 10 - (num % 11)] = ledColor_rgb;
	}
	else {
		switch (num) {
		case 110:
			leds[111] = ledColor_rgb;
			break;
		case 111:
			leds[112] = ledColor_rgb;
			break;
		case 112:
			leds[113] = ledColor_rgb;
			break;
		case 113:
			leds[110] = ledColor_rgb;
			break;
		case 114:
			leds[114] = ledColor_rgb;
			break;
		default:
			break;
		}
	}
#endif // LED_RGB
#ifdef LED_RGBW
	if (num < 110) {
		if ((num / 11) % 2 == 0) {
			leds[num * 2] = ledColor_wbg;
			leds[num * 2 + 1] = ledColor_r;
		}
		else {
			leds[(((num / 11) * 11) + 10 - (num % 11)) * 2] = ledColor_wbg;
			leds[(((num / 11) * 11) + 10 - (num % 11)) * 2 + 1] = ledColor_r;
		}
	}
	else {
		switch (num) {
		case 110:
			leds[111 * 2] = ledColor_wbg;
			leds[111 * 2 + 1] = ledColor_r;
			break;
		case 111:
			leds[112 * 2] = ledColor_wbg;
			leds[112 * 2 + 1] = ledColor_r;
			break;
		case 112:
			leds[113 * 2] = ledColor_wbg;
			leds[113 * 2 + 1] = ledColor_r;
			break;
		case 113:
			leds[110 * 2] = ledColor_wbg;
			leds[110 * 2 + 1] = ledColor_r;
			break;
		case 114:
			leds[114 * 2] = ledColor_wbg;
			leds[114 * 2 + 1] = ledColor_r;
			break;
		default:
			break;
		}
	}
#endif // LED_RGBW
#endif // LED_LAYOUT_HORIZONTAL

#ifdef LED_LAYOUT_VERTICAL
#ifdef LED_RGB
	uint8_t ledNum;
	if (num < 110) {
		if ((num / 10) % 2 == 0) ledNum = num;
		else ledNum = ((num / 10) * 10) + 9 - (num % 10);
		if (ledNum < 10) leds[ledNum + 1] = ledColor_rgb;
		else if (ledNum < 100) leds[ledNum + 2] = ledColor_rgb;
		else leds[ledNum + 3] = ledColor_rgb;
	}
	else {
		switch (num) {
		case 110:
			leds[0] = ledColor_rgb;
			break;
		case 111:
			leds[102] = ledColor_rgb;
			break;
		case 112:
			leds[113] = ledColor_rgb;
			break;
		case 113:
			leds[11] = ledColor_rgb;
			break;
		case 114:
			leds[114] = ledColor_rgb;
			break;
		default:
			break;
		}
	}
#endif // LED_RGB
#ifdef LED_RGBW
	uint8_t ledNum;
	if (num < 110) {
		if ((num / 10) % 2 == 0) ledNum = num;
		else ledNum = ((num / 10) * 10) + 9 - (num % 10);
		if (ledNum < 10) {
			leds[(ledNum + 1) * 2] = ledColor_wbg;
			leds[(ledNum + 1) * 2 + 1] = ledColor_r;
		}
		else {
			if (ledNum < 100) {
				leds[(ledNum + 2) * 2] = ledColor_wbg;
				leds[(ledNum + 2) * 2 + 1] = ledColor_r;
			}
			else {
				leds[(ledNum + 3) * 2] = ledColor_wbg;
				leds[(ledNum + 3) * 2 + 1] = ledColor_r;
			}
		}
	}
	else {
		switch (num) {
		case 110:
			leds[0 * 2] = ledColor_wbg;
			leds[0 * 2 + 1] = ledColor_r;
			break;
		case 111:
			leds[102 * 2] = ledColor_wbg;
			leds[102 * 2 + 1] = ledColor_r;
			break;
		case 112:
			leds[113 * 2] = ledColor_wbg;
			leds[113 * 2 + 1] = ledColor_r;
			break;
		case 113:
			leds[11 * 2] = ledColor_wbg;
			leds[11 * 2 + 1] = ledColor_r;
			break;
		case 114:
			leds[114 * 2] = ledColor_wbg;
			leds[114 * 2 + 1] = ledColor_r;
			break;
		default:
			break;
		}
	}
#endif // LED_RGBW
#endif // LED_LAYOUT_VERTICAL
}

void LedDriver::clear() {
	FastLED.clear();
}

void LedDriver::setBrightness(uint8_t ledBrightness) {
	FastLED.setBrightness(ledBrightness);
}

void LedDriver::getBrightness() {
	FastLED.getBrightness();
}

void LedDriver::show() {
	FastLED.show();
}

// Calculate white
uint8_t LedDriver::getWhite(uint8_t red, uint8_t green, uint8_t blue) {
	uint8_t low = min(red, min(green, blue));
	uint8_t high = max(red, max(green, blue));
	uint8_t saturation = round(100 * ((high - low) / high));
	return (255 - saturation) / 255 * (red + green + blue) / 3;
}
