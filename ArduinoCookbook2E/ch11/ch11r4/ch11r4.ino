/*
  Scroll
  * this sketch scrolls text left when tilt switch closes
  * text scrolls right when not tilted.
 */

#include <LiquidCrystal.h>

// initialize a new object with the numbers of the interface pins
LiquidCrystal lcd(12,11,4,5,6,7);
const int numRows = 2;
const int numCols = 8;

const int tiltPin = 15; // A1 used as dig input -- pin connected to 'tilt sensor' or switch to ground

const char textString[] = "tilt to scroll left";
const int textLen = sizeof(textString) -1; // the number of characters
bool isTilted = false;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
  pinMode(tiltPin, INPUT);
  digitalWrite(tiltPin, HIGH); // turn on pull-ups for the tilt sensor
  lcd.print(textString);
}

void loop()
{
  if(digitalRead(tiltPin) == LOW && isTilted == false )
  {
    // here if tilted left so scroll text left
    isTilted = true;
    for (int position = 0; position < textLen; position++)
    {
      lcd.scrollDisplayLeft();
      delay(150);
    }
  }
  if(digitalRead(tiltPin) == HIGH && isTilted == true )
  {
    // here if previously tilted but now flat, so scroll text right
    isTilted = false;
    for (int position = 0; position  < textLen; position++)
    {
      lcd.scrollDisplayRight();
      delay(150);
    }
  }
}
      
    
