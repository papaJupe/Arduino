/*
 * Marquee using lcd.scrollDisplayLeft, mead mod for 2x8 screen
 * this sketch repeatedly scrolls up to __ characters of text (my 2x8 takes up to 40 bytes)
 * if too many it spills to next line
*/

#include <LiquidCrystal.h>

// initialize new object with the numbers of the interface pins
LiquidCrystal lcd(12,11,4,5,6,7);
const int numRows = 2;
const int numCols = 8;


void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
}

void loop()
{
  marquee("A message too long to fit..........");  // = more than 32 bytes
  delay(1000);
  lcd.clear();
}

// this function uses scrolling to display a message up to 32 bytes long or more if LCD allows
void marquee( char *text)
{  
  lcd.print(text);
  delay(1000);
  for (int position = 0; position < strlen(text)-numCols; position++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }    
}

