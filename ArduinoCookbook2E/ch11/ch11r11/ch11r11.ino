/*
  TellyMate
  Simple demo for TellMmate Shield
*/

const byte ESC = 0x1B;  // ASCII escape character used in TellyMate commands

void setup()
{
  Serial.begin(57600); //57k6 baud is default TellyMate speed
  clear();  // clear the screen
  Serial.print("   TellyMate Character Set"); // write some text
  delay(2000);
}

void loop()
{

  byte charCode = 32;    // characters 0 through 31 are control codes
  for(int row=0; row < 7; row++) // show 7 rows
  {
    setCursor(2, row + 8); // center the display
    for(int col= 0; col < 32; col++) // 32 characters per row
    {
       Serial.print(charCode);
       charCode = charCode + 1;
       delay(20);
    }
  }
  delay(5000);
  clear();
}

// TellyMate helper functions

void clear( )  // clear the screen
{ // <ESC>E
  Serial.print(ESC);
  Serial.print('E');
}

void setCursor( int col, int row) // set the cursor
{ // <ESC>Yrc
  Serial.print(ESC);
  Serial.print('Y' ) ;
  Serial.print((unsigned char)(32 + row)) ;
  Serial.print((unsigned char)(32 + col)) ;
}