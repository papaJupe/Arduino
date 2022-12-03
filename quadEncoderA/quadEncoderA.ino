/*
   quadEncoder read A,B channel of 5 v output encoder, display on 8x2 lcd and/or print
   pin D8 button to zero encoderPos
*/
#include <elapsedMillis.h>
elapsedMillis timeElapsed; // declare global so not reset every loop
// could you declare static in loop?
int interval = 333;  // ms

#define encoder0PinA 2
#define encoder0PinB 3
#define zeroCountPin 8
volatile int encoder0Pos = 0;
int tmp = 0;
int oldTmp = 0;
int Aold = 0;
int Bnew = 0;


// include the library code for the LCD
#include <LiquidCrystal.h>
// initialize 8x2 display with needed Arduino pins
// lib is smart enough to know if you're using 4 signal pins or 8
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

void setup()
{
  // set up the LCD's number of columns and rows
  lcd.begin(8, 2);
  // init Serial Connection
   Serial.begin(9600);
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(zeroCountPin, INPUT_PULLUP);
  // encoder pinA goes to interrupt 0 (pin 2 on Uno); value change calls method A
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pinB goes to interrupt 1 (Uno pin 3); value change calls method B
  attachInterrupt(1, doEncoderB, CHANGE);
  // print to 2 lines of LCD, if present
  lcd.clear();  // clear screen and set cursor to (0,0)
  lcd.print("setup");
  lcd.setCursor(0, 1);  // second line
  lcd.print("pos=0");
  //    lcd.print(voltage);
  // Serial.println("setup done");
  delay(2000);
}

void loop() {
  // suspend ISR while getting position value, v. ardu cookbk ch 6.12

  static int avgPos = 0;
  uint8_t oldSREG = SREG;
  cli();
  tmp = encoder0Pos;
  SREG = oldSREG;   // awaken ISR

  // smooth value w/ expon MA, update all var every loop
  avgPos = round(oldTmp * 0.8 + tmp * 0.2);

  if (!digitalRead(zeroCountPin)) {  // if button pressed
    encoder0Pos = 0;
    oldTmp = 0;
    avgPos = 0;
    lcd.clear();  // clear screen and set cursor to (0,0)
    lcd.print("pos:");
     // Serial.println("button press");
    lcd.setCursor(0, 1);  // second line
    lcd.print(avgPos);
  }

  // serial/lcd print position ~3x/sec
  else if (timeElapsed > interval) {
    // Serial.println(avgPos, DEC);
    // print to 2 lines of LCD, if present
    lcd.clear();  // clear screen and set cursor to (0,0)
    lcd.print("pos =");
    lcd.setCursor(0, 1);  // second line
    lcd.print(avgPos);
    //    lcd.print(voltage);

    timeElapsed = 0;
  } // end if
  
  oldTmp = avgPos;
  delay(100);
}

// Interrupt on A changing state; _XOR_ compares B's current state with
// A's former state, so the XOR is always true if CW rotation, false if CCW
void doEncoderA() {
  Bnew^Aold ? encoder0Pos++ : encoder0Pos--;
  Aold = digitalRead(encoder0PinA);
}

// Interrupt on B changing state; function compares B new (current) state
// with existing A state, so same logic: XOR is true if CW, false if CCW
void doEncoderB() {
  Bnew = digitalRead(encoder0PinB);
  Bnew^Aold ? encoder0Pos++ : encoder0Pos--;
}
