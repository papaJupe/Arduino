/*
DFRobot LCD Shield DFR0009 and similar from Sainsmart, et al
Key Grab v0.2
Written by Glendon Klassen
gjklassen@gmail.com
http://www.sourceforge.net/users/ecefixer
http://ecefixer.tumblr.com

Displays the currently pressed key on the 2 row 16 col. LCD screen

Key Codes (in left-to-right order):

None   - 0  (actually -1 ?)
Select - 1
Left   - 2
Up     - 3
Down   - 4
Right  - 5

*/

#include <LiquidCrystal.h>
#include <LCD_DFR_Keypad.h>   // voltage params in .cpp file tuned for my panel

//Pin assignments for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

DFR_Key keypad;

int localKey = 0;
String keyString = "";
                 
void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Key Grab v0.2");
  delay(2500);
  
  /*
  OPTIONAL
  keypad.setRate(x);
  Sets the sample rate at once every x milliseconds.
  Default: 10ms
  */
  keypad.setRate(10);

} // end setup

void loop() 
{ 
  /*
  keypad.getKey();    Grabs the current key.
  Returns a non-zero integer corresponding to the pressed key,
  OR   Returns 0 for no keys pressed,
  OR   Returns -1 (sample_wait) when no key is available to be sampled
   -- why are these not the same ???
  */
  
  localKey = keypad.getKey();   // comes back from .cpp processing
  int anal = analogRead(A0);  // use to tune params to your particular values for switches
  if (localKey != SAMPLE_WAIT)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current Key:");
    lcd.setCursor(0, 1);
    lcd.print(anal); // for tuning, or for other sketch's use of V
    // lcd.print(localKey);  
  }
  delay (20);
}
