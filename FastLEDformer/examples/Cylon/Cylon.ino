#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 64 

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
//#define CLOCK_PIN 13

// Define the array of CRGB structs for the leds
CRGB leds[NUM_LEDS];

void setup() 
{ 
	Serial.begin(57600);
	Serial.println("resetting");
	LEDS.addLeds<WS2812B,DATA_PIN,GRB>(leds,NUM_LEDS);
	LEDS.setBrightness(84);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() 
{ 
	static uint8_t hue = 0;
	Serial.print("x");
	// First slide the led in one direction starting at 0
	for(int i = 0; i < NUM_LEDS; i++) 
		{
			// Start at red then modulate as you loop thru all
			leds[i] = CHSV(hue++, 255, 255);
			// Show the leds
			FastLED.show(); 
			// now that we've shown the leds, reset the i'th led to black
			// leds[i] = CRGB::Black;
			fadeall();
			// Wait a little bit before we go back
			delay(10);
		}
	Serial.print("x");  // need to reset Ser Mon to this # to see the "x"

	// Now go in the other direction starting at the far end
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Start @ red ?
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've momentarily flashed all, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again from the start
		delay(10);
	}
}
