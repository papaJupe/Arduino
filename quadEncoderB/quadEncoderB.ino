/* v. A for 8x2 display, v. B for 16x2 display
  quadEncoder read A,B channel of 5 v output encoder, display on lcd and/or print
  read V on pin A0 (any button works) to zero encoderPos
  use: wire encoder 5v and ground to Ardu's, A, B channel to Ardu pin 2,3
*/
#include <elapsedMillis.h>
elapsedMillis timeElapsed; // declare global so not reset every loop
// could you declare static in loop?
int interval = 333;  // ms

#define encoder0PinA 2
#define encoder0PinB 3
#define zeroCountPin 14 // A0
volatile int encoder0Pos = 0;
int tmp = 0;
int oldTmp = 0;
int Aold = 0;
int Bnew = 0;


// include library for the LCD (& keypad lib if needed)
#include <LiquidCrystal.h>

//Pin assignments for DFRobot LCD 16x2 Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup()
{
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  // init Serial Connection PRN vs. comment out
  // Serial.begin(9600);
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(zeroCountPin, INPUT_PULLUP);
  // encoder pinA goes to interrupt 0 (Uno pin 2); value change calls method A
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pinB goes to interrupt 1 (Uno pin 3); value change calls method B
  attachInterrupt(1, doEncoderB, CHANGE);
  // Serial.println("setup done");

  // print to 2 lines of LCD, if present
  lcd.clear();  // clear screen and set cursor to (0,0)
  lcd.print("setup done");
  lcd.setCursor(0, 1);  // second line
  lcd.print("position=0");
  //    lcd.print(voltage);
  delay(2000);
}

void loop() {
  // suspend ISR while getting position value, v. ardu cookbk ch 6.12
  static int avgPos = 0;
  uint8_t oldSREG = SREG;
  cli();
  tmp = encoder0Pos;
  SREG = oldSREG;  // awaken ISR

  // smooth value w/ expon MA; oldTmp reset after each printout 3x/sec
  avgPos = round(oldTmp * 0.8 + tmp * 0.2);

  if (analogRead(zeroCountPin) < 800) {  // button pressed
    encoder0Pos = 0;
    oldTmp = 0;
    avgPos = 0;
    // Serial.println(analogRead(zeroCountPin));
    //    Serial.println("avgPos= " + String(avgPos));
    //    Serial.println("oldTmp= " + String(oldTmp));
    //    Serial.println("tmp= " + String(tmp));

    lcd.clear();  // clear screen and set cursor to (0,0)
    lcd.print("position:");
    lcd.setCursor(0, 1);  // second line
    lcd.print(avgPos);
  }
  // serial/lcd print position if average changed ~3x/sec
  else if (timeElapsed > interval) {
    // Serial.println(avgPos, DEC);

    // print to 2 lines of LCD, if present
    lcd.clear();  // clear screen and set cursor to (0,0)
    lcd.print("position:");
    lcd.setCursor(0, 1);  // second line
    lcd.print(avgPos);

    timeElapsed = 0;
  } // end if
  
    oldTmp = avgPos;
  delay(100);
}

// Interrupt on 'A' changing state; _XOR_ compares B's current (unchanged) state
// with A's former state, so the XOR is always true if CW rotation, false if CCW
void doEncoderA() {
  Bnew^Aold ? encoder0Pos++ : encoder0Pos--;
  Aold = digitalRead(encoder0PinA);
}
// Interrupt on 'B' changing state; function compares B new (current) state with existing
// A state, so same logic as above: _XOR_ is true if CW, false if CCW
void doEncoderB() {
  Bnew = digitalRead(encoder0PinB);
  Bnew^Aold ? encoder0Pos++ : encoder0Pos--;
}
