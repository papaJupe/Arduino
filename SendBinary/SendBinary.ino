/*
SendBinary, major mod from orig Cookbook ch4r6 which was badly flawed
   for printing to ser. monitor, but may have worked for other devices that expect char
   over serial.  Sends a header followed by two random integer values as binary data.
   Uses: random, lowByte, highByte, Serial.write, BIN format
*/

int intValue;    // an integer value (for Ardu, 16 bits, 2 bytes)

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  
  while (Serial.available() != 1)   // stop when any char entered, so it doesn't print forever
  {
  Serial.print('H');    // send a header character
  Serial.print('\n');    // a new line
   
  intValue = random(599); // generate a random number between 0 and 599
  
  Serial.print("your new Rand is: "); Serial.println(intValue);   // show what it is
 
// orig: send the two bytes that comprise an integer; .write doesn't work for ser. mon. -- always prints byte as char;
//  Serial.write(lowByte(intValue));  // send the low byte   -- some software may expect to get bytes in this way
//  Serial.write(highByte(intValue)); // send the high byte  --     i.e. ascii char format, without any spaces

  Serial.print(lowByte(intValue),BIN);                              // print the low byte binary
  Serial.print("  "); Serial.println(highByte(intValue),BIN); // print a space and the high byte

  // get another random integer

 intValue = random(2100); // generate a random number between 0 and 2099
 Serial.print("your 2nd Rand is: "); Serial.println(intValue);   // show what it is

  // send the two bytes of new int
  Serial.print(lowByte(intValue),BIN);  // send the low byte binary
  Serial.print("  "); Serial.println(highByte(intValue),BIN); // print space and the high byte binary
  Serial.print("print the ascii value of low one: "); Serial.println(lowByte(intValue)); // byte will print as ascii # int
  Serial.write("write the low byte as char: "); Serial.write(lowByte(intValue)); Serial.write('\n'); // send the low byte
                // write always prints char value of byte or int to Ser Mon
  Serial.print('\n');  // a new line
  delay(1000);
  } // end while
}  // end loop