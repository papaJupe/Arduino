#include <Streaming.h>

void setup()
{
  Serial.begin(9600);
  int lettera = 'A';
  int month = 4, day = 17, year = 2009;
 
 // a non-stream way to concat str and var
  Serial.println("stream lib vers. is " + String(STREAMING_LIBRARY_VERSION));
  
  //orig had line after line of Serial xxx, but this shows it better
  Serial << "This is an example of the new streaming" << endl\
  << "library.  This allows you to print variables" << endl\
  << "and strings without having to type line after" << endl\
<< "line of Serial.print() calls." << "\n" <<  "Examples: " << endl;
  
  Serial << "A is " << lettera << "." << endl;
  Serial << "The current date is " << day << "-" << month << "-" << year << "." << endl;
  
  Serial << "You can use modifiers too, for example:" << endl; // orig _BYTE now _BIN
    // but I edited streaming.h so _BYTE goes to _BIN
  // "\" escapes the return at endline ; 
  // these numeric prefixes defined in lib; without the char cast int 'A' is 65
  Serial << "char " << (char)lettera << " is " << _BYTE(lettera) << " in binary,\
 & " << _HEX(lettera) << " in hex (missing its 0x)." << endl; // ? how to get proper hex to print 0x__
 
}

void loop()
{}
