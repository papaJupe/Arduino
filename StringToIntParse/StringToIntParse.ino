/*
 StringToIntParse -- String to Integer conversion

 Reads serial input chars one by one, putting all digits (isDigit()) into a String; if input is Enter, 
 seen as a newline, then prints the String; Ser. Mon. must be set to send 'newline' for this to work.
 Simply parses all digits from input; not limited to size of int var.

Uses: Serial.read, string.toInt, isDigit

 created 29 Nov 2010
 by Tom Igoe

 */

String inString = "";    // String object to hold input

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
    {
      ; // wait for serial port to connect. Needed for native USB port only
    }

  // send an intro:
  Serial.println("port open: type some chars, then Enter -- will parse for a number");
  Serial.println();
}  // end setup

void loop() 
{ 
  while(!Serial.available());   // hold action here until some input
  delay(10);  // this is NECESSARY for next while loop to work
  // Read serial input:
  while (Serial.available() > 0) 
  {
    int inChar = Serial.read();  // can be int, char, byte type; if int it's ascii # of byte
    // Serial.print(" got: "), Serial.println(inChar); shows different types; all act same below
    if (isDigit(inChar)) 
      {
      // cast the incoming byte (or its ascii #) to a char and add it to the String:
      inString += (char)inChar; // w/o cast you'll put ascii number in String
      } // end if
    // if you get a newline, print int & plain String (they are the same of course)
//    if (inChar == '\n')  // (must enable 'newline' ending to send from Ser. Mon.)
//      {
//      Serial.print("String.toInt: ");
//      Serial.println(inString.toInt());
//      Serial.print("String: ");
//      Serial.println(inString);
//      // clear the string for new input:
//      inString = "";
//      }  //end if

  }  //end while ser. avail
  // to print when while loop runs out of input use this instead:
  if(inString !="")
    {
        Serial.print("String.toInt: ");
        Serial.println(inString.toInt());
        // clear the string for new input:
        //  inString = "";
    }
   else Serial.println("invalid entry"); // no digits seen --> empty string
   inString = "";
   Serial.println("try again?");
} // end loop
