// sketch 09-01 USB Message Board, mead mod for 2x8 display on shield


#include <LiquidCrystal.h>

// lcd(RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12,11,4,5,6,7);

int numCols = 8;
int numRows = 2;

void setup()
{
  Serial.begin(9600);
  lcd.begin(numCols,numRows);  // monk book had this backwards
  lcd.clear();
  lcd.setCursor(0,0);    // 1st col, 1st row
  lcd.print("Arduino");
  lcd.setCursor(0,1);  // 1st col, 2nd row
  lcd.print("Rules");
//  lcd.autoscroll();  //doesn't show new char until it fills 40 char buffer???
}

void loop()
{
  if (Serial.available() > 0) 
  { 
      
    char ch = Serial.read();
    if (ch == '.')
    {
      lcd.clear();      //clears and brings cursor home; home puts cursor at 0,0 but doesn't clear screen
    }
    else if (ch == '/')
    {
      // new line
      lcd.setCursor(0, 1);
    }
    else
    {
      lcd.write(ch);
    }
  }
}  // end loop


