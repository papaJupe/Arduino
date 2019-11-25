
/* FastLEDparseInt
    Goal-- display on/off states of 16 element IR array on string
    of 16 fastLEDs.
    1st test: parseInt sketch tests ints input from Ser Mon to turn on
    specific lites, 0-15, from Mega.
    2nd test: trackIR sketch reads array's 16 H/L inputs into an int (bitWrite),
    then writes 0/1 values from the int to corresponding lites of string.

    bigMega2 (this) adds: second LED string; serial input to loop to send N 
    pulses to control array emitter power;

    ToDo: locate first edge of ON lites, send as # (0-15) over dig.
    outputs (x 4) to roboRIO. Send second number for rear array.
    
    Uses: FastLED, pinMode(pin#, INPUT), digitalRead(), bitWrite(int n,bit#,value),
    bitRead(int n,bit#) -- bit #: bit 0 is rightmost, least signif. bit, printBits()
    function to display bits in int variables
*/


#include "FastLED.h"
// How many leds to control?
#define NUM_LEDS 35   // would need 32 to show 2 16 bit array values
#define BRIGHTNESS 12
#define DATA_PIN 2
#define ePin 61     // A7 on Mega is 61 -- control signal for dimming
#define LED_TYPE WS2812B

// create leds color array
CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(9600);
  // init pins to read sensors
  for (int k = 0; k < 32; k++)
  {
    pinMode(22 + k, INPUT); // erratic reads if floating
    //
    // on big Mega left 16 pin row on R side of bd is one sensor, rt is other
  }

  pinMode(ePin, OUTPUT);
  digitalWrite(ePin, HIGH);  // default is full emitter power

  delay(1000);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );

}  // end setup

void loop()
{
  static byte ct = 0;  // used for print sanity only
  static int front = 0; // 16 bit ints hold input from front/rear IR arrays;
  static int rear = 0;

  // if binary of these int vars are all 0 all lites are black

  // input a # (0-31) from Ser Mon to reset emitter power; 0 is full power, 31 lowest
  if (Serial.available() > 0)
  {
    delay(20);
    int e = Serial.parseInt();
    // resetting emitter power, copied from lib's.cpp
    noInterrupts();
    for (int i = 0; i < e + 1; i++)
    {
      delayMicroseconds(1);
      digitalWrite(ePin, LOW);
      delayMicroseconds(1);
      digitalWrite(ePin, HIGH);
    }
    interrupts();

    // print input back to Ser Mon
    Serial.println();
    Serial.print("power reset to "); Serial.println(e);
    Serial.println();
  }  //end if serial.avail

  // while (Serial.read() != -1); // clear the input buffer (should be already)

  // loop reads 16 DIs from IR array at Mega pins n-->n+15,
  // write 1/0 to bits 15-->0 of int variable (hi(15) to lo(0) bits)

  // pin numbering varies mini vs. big Mega
  // pin order on sensor is reversed: left is 16, right end is 1
  for (int i = 0; i < 16; i++) // start reading 22 on big Mega then even pins;

  { // assumes pin pulled low by sensed reflection
    // if pin is low put 1 in the int variable

    bitWrite(front, 15 - i, (digitalRead((i * 2) + 22) == 0));

    // rear 16 element array starts @ pin 23 on big Mega (right row)
    // then odd pins
    bitWrite(rear, 15 - i, (digitalRead((i * 2) + 23) == 0));
  }

  if (ct == 2) {
    // use printBits fx() to display leading zeros and spaces between nibbles
    Serial.print("front values: ");
    printBits(front);
    Serial.println();
    Serial.print("rear values: ");
    printBits(rear);
    Serial.println();
  }
  // set all lites black
  for (int j = 0; j < 35; j++)
  {
    leds[j] =  CRGB(0, 0, 0);
  }

  // make some red if a 1 is at corresponding position in int
  for (int j = 0; j < 16; j++)
  {
    if (bitRead(front, 15 - j))
      leds[j] = CRGB::Red;

    if (bitRead(rear, 15 - j))
      leds[33 - j] = CRGB::Red;
  }

  FastLED.show();  // send colors

  ct++;  // ct just used for printing every 4 sec.
  if (ct == 20) ct = 0;

  delay(100);

}  // end loop

// prints N-bit integer in this form: 0000 0000 0000 0000
// works for 4 - 32 bits & accepts signed numbers
void printBits(uint32_t n) // 16 bit int param -- should NOT need uint32 here ???
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
