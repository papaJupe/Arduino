// cookbook 12-3 using pulseIn to capture/display an R/C pulse width
// fx usable with any digital pin
// mead mod for 16x2 lcd display, LED output pin, etc
// Connect signal from R/C receiver into Ardu's digital pin X
// Turn On R/C transmitter
// If in-range signal is received, you should see the LED turn On.
// If not in range, LED is turned Off.

#include <LiquidCrystal.h>

int my_led = 15; // A1 drives LED
unsigned long pulse_val;  // never over 2400 for RC but function handles much longer

// make DFRobot LCD object, with Pins to use
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // order matters

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("lcd alive");
  delay(2000);
 
  pinMode(my_led, OUTPUT); // led + on A1
  pinMode(13,INPUT);  // defaults to outpt (onboard LED)
//  pinMode(16, OUTPUT); // can use A2 for LED ground
//  digitalWrite(16, LOW);

} // end setup


void loop()
{
  pulse_val = pulseIn(13, HIGH);  // record width of high pulse from Dx
  
  if (pulse_val > 1200 && pulse_val < 1800)  // middle of range lites up, else dark
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
  lcd.print(pulse_val);
  delay(100);
}  //end loop

