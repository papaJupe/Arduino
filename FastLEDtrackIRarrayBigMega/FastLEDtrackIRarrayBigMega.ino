/* FastLEDparseInt
    Goal-- display on/off states of 16 element IR array on string
    of 16 fastLEDs. 
    1st test: parseInt sketch tests ints input from Ser Mon to turn on
    specific lites, 0-15, from Mega.
    trackIR sketch reads array's 16 H/L inputs into an int (bitWrite),
    then writes 0/1 values from the int to corresponding lites of string.
    bigMega (this) started to revise for second sensor input going to rear
    LED string

    Uses: FastLED, pinMode(pin#, INPUT), digitalRead(), bitWrite(int n,bit#,value),
    bitRead(int n,bit#) -- bit #: bit 0 is rightmost, least signif. bit, printBits()
    function to display bits in int variables
*/


#include "FastLED.h"
// How many leds to control?
#define NUM_LEDS 16   // would need 32 to show 2 16 bit array values
#define BRIGHTNESS 12
#define DATA_PIN 2
#define LED_TYPE WS2812B

// create leds color array
CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(9600);

  for (int k = 0; k < 32; k++)
  {
    pinMode(22 + k, INPUT_PULLUP); // erratic reads if floating; 
    // ? need if analog source
     // on big Mega left 16 pin row is one sensor, rt is other
  }
  delay(1000);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );

}  // end setup

void loop()
{
  static byte ct = 0;  // used for print sanity only
  static int front = 0; // 16 bit ints hold input from front/rear IR arrays;
  static int rear = 0;

  // 0 if all are black, could set this later to -1 to indicate invalid input

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

  // loop reads 16 DIs from IR array at Mega pins n-->n+15,
  // write 1/0 to bits 15-->0 of int variable (hi(15) to lo(0) bits)

  // pin numbering varies mini vs. big Megas

  for (int i = 0; i < 16; i++) // start reading 22 on big Mega then even pins;

  { // assumes pin pulled low by active sensor element
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

  // set string LED on/off acccording to value of bit at position in int
  for (int j = 0; j < NUM_LEDS; j++)
  {
    if (bitRead(front, 15 - j))
      leds[j] = CRGB::Red;

    else leds[j] =  CRGB(0, 0, 0);
  }

  // second string might need reversed values if continuous with first, but oppos. direction

  FastLED.show();  // send colors

  ct++;  // ct just used for printing every 4 sec.
  if (ct == 20) ct = 0;

  delay(200);

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
