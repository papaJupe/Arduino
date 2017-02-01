// Code Example 1-3 Using an Interrupt pin to capture an R/C pulse length
// mead mod for 16x2 lcd display, different LED output pin, etc
// Connect signal from R/C receiver into Ardu's digital pin 2/3
// Turn On R/C transmitter
// If valid signal is received, you should see the LED on pin x turn On.
// If no valid signal is received, you will see the LED turned Off.

#include <LiquidCrystal.h>

int my_led = 15; // A1 drives LED

volatile long servo_startPulse;
volatile unsigned int pulse_val;  // never over 2400
int servo_val;  // will = pulse val for display, etc


// make DFRobot LCD object, give it Pins to use
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // order matters

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("lcd alive");
  delay(2000);
 
  pinMode(my_led, OUTPUT); // led + on A1
//  pinMode(16, OUTPUT); // can use A2 for LED ground
//  digitalWrite(16, LOW);

  attachInterrupt(1, rc_begin, RISING); // init D3 as interrupt on a rising signal
} // end setup

// set up the rising interrupt
void rc_begin()
{
  servo_startPulse = micros();
  detachInterrupt(1);  // turn Off the rising interrupt
  attachInterrupt(1, rc_end, FALLING); // turn On the falling interrupt
}

// set up the falling interrupt
void rc_end()
{
  pulse_val = micros() - servo_startPulse;
  detachInterrupt(1);  // turn Off the falling interrupt
  attachInterrupt(1, rc_begin, RISING); // turn On the rising interrupt
}

void loop()
{
  servo_val = pulse_val; // get the value from Interrupt Service Routine
  if (servo_val > 1200 && servo_val < 1800)  // middle of range lites up, else dark
  {
    digitalWrite(my_led, HIGH);   // if the value is mid- R/C range, turn the LED On
  }  // end if
  else
  { 
    digitalWrite(my_led, LOW);  // If value is not within the range, turn the LED Off.
  }
  lcd.clear();  // display value on LCD
  lcd.setCursor(0, 0);
  lcd.print("pulse val: ");
  // lcd.setCursor(0, 1);
  lcd.print(servo_val);
  delay(100);
}  //end loop

