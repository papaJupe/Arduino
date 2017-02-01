/*
  Marquee
  * this sketch can scroll a very long line of text
 */

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int numRows = 2;
const int numCols = 16;


void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
}

void loop()
{
  marquee("A message too long to fit !");
  delay(1000);
  lcd.clear();
}

// this version of marquee uses manual scrolling for very long messages
void marquee( char *text)
{
  int length = strlen(text); // the number of characters in the text
  if(length < numCols)
    lcd.print(text);
  else
  {
    int pos;
    for( pos = 0; pos < numCols; pos++)
      lcd.print(text[pos]);
    delay(1000); // allow time to read the first line before scrolling
    pos=1;
    while(pos <= length - numCols)
    {
      lcd.setCursor(0,0);
      for( int i=0; i < numCols; i++)
        lcd.print(text[pos+i]);
      delay(300);
      pos = pos + 1;
    }
  }
}
      
    
