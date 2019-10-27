/* FastLEDparseInt
    Goal-- display on/off states of 16 element IR array on string
    of 16 fastLEDs. parseInt sketch tests ints input from Ser Mon to turn on
    specific lites, 0-15, from Mega.
    trackIR sketch (this) reads array's 16 H/L inputs into an int (bitWrite),
    then writes 0/1 values from the int to corresponding lites of string.
    Third, locate first edge of ON lites, send as # (0-15) over dig.
    outputs (x 4) to roboRIO. Send second number for rear array.

    Uses: FastLED, pinMode(pin#, INPUT), digitalRead(), bitWrite(int n,bit#,value),
    bitRead(int n,bit#) -- bit #: bit 0 is rightmost, least signif. bit. 
*/


#include "FastLED.h"
// How many leds to control?
#define NUM_LEDS 16
#define BRIGHTNESS 12
#define DATA_PIN 2
#define LED_TYPE WS2812B

// create leds color array
CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(9600);

  for (int k = 0; k < 16; k += 2)
  {
    pinMode(55 + k, INPUT_PULLUP); // erratic reads if floating
    pinMode(33 + k, INPUT_PULLUP);
  }
  delay(1000);

  // Uncomment/edit one of the following lines for your string type
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );

}  // end setup

void loop()
{
  static byte ct = 0;
  static int c = 0; // 16 bit int holds input from arrays; if 0 all
  // are black, could set this later to -1 to indicate invalid input

  // read input from Ser Mon
  // if (Serial.available() > 0)
  //  {
  //    delay(20);
  //    //    Serial1.readBytes(inByt, 5); // incoming bytes --> inByt array
  //    //    Serial.print("inByt using .write ");
  //    //    Serial.write(inByt, 5);
  //    c = Serial.parseInt();
  //    // print input back to Ser Mon
  //    Serial.print("input is "); Serial.println(c);
  //  }  //end if serial.avail

  // while (Serial.read() != -1); // clear the input buffer (should be already)

  // loop reads 16 DIs from Mega pins n-->n+, from IR array
  // write 1/0 to bits 15-->0 of int c (hi to lo bits)
  for (int i = 0; i < 8; i++) // start reading A1 (55 on Mega) then 57-69 (odd)
  { // assumes pin pulled low by active sensor element
    bitWrite(c, 15 - i, (digitalRead((i * 2) + 55) == 0));
    // array B wires go to pins 33-47 (odd pins)
    bitWrite(c, 7 - i, (digitalRead((i * 2) + 33) == 0));
  }

  if (ct == 2) {
    // use fx() to display leading zeros and spaces between nibbles
    printBits(c);
    Serial.println();
  }
  
  // set string LED on/off acccording to value of bit at position in int
  for (int j = 0; j < NUM_LEDS; j++)
  {
    if (bitRead(c, 15 - j))
      leds[j] = CRGB::Red;

    else leds[j] =  CRGB(0, 0, 0);
  }
  
  FastLED.show();  // send colors
  
  ct++;  // just used for printing every 4 sec.
  if (ct == 20) ct = 0;

  delay(200);

}  // end loop

// prints N-bit integer in this form: 0000 0000 0000 0000
// works for 4 - 32 bits & accepts signed numbers
void printBits(uint32_t n) // 16 bit int param should NOT need uint32 here ???
  // but any smaller and it fails to print highest bit
{
  byte numBits = 16;  // 2^numBits must be big enough to include the number n
  char b;
  char c = ' ';   // spacer character
  for (byte i = 0; i < numBits; i++) {
    // shift 1 and mask to identify each bit value
    b = (n & (1 << (numBits - 1 - i))) > 0 ? '1' : '0'; 
    // slightly faster to print chars than ints (saves conversion)
    Serial.print(b);
    // print a separator between every 4 bits
    if (i < (numBits - 1) && ((numBits - i - 1) % 4 == 0 )) 
         Serial.print(c); 
  }
}  // end printBits
