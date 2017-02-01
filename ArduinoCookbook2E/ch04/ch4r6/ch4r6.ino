/*
 * SendBinary sketch, major mod from orig which was badly flawed for printing to ser. monitor
 * Sends a header followed by two random integer values as binary data;
 */

int intValue;    // an integer value (16 bits, 2 bytes)

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  while (Serial.available() != 1)   // stop doing stuff when any char entered 
                                    // (and not read); loop keeps going but no action ensues
  {

    Serial.print('H'); // send a header character
    Serial.print('\n');  // a new line

    // send a random integer
    intValue = random(599); // generate a random number between 0 and 599

      Serial.print("your new Rand is: "); 
    Serial.println(intValue);   // show what it is

    // orig: send the two bytes that comprise an integer; doesn't work 4 ser. mon. which always prints byte as char;
    //  Serial.write(lowByte(intValue));  // send the low byte -- but some software may expect to get bytes in this
    //  Serial.write(highByte(intValue)); // send the high byte -- i.e. (binary vs char?) format without any spaces

    Serial.print(lowByte(intValue),BIN);  // print the low byte as binary
    Serial.print("  "); 
    Serial.println(highByte(intValue),BIN); // print a space and the high byte

    // get another random integer

    intValue = random(2100); // generate a 2nd random number between 0 and 1099
    Serial.print("your 2nd Rand is: "); 
    Serial.println(intValue);   // show what it is

    // send the two bytes of new int
    Serial.print(lowByte(intValue),BIN);  // print the low byte binary
    Serial.print("  "); 
    Serial.println(highByte(intValue),BIN); // print space and the high byte binary

    Serial.print("serPrint the int value of low byte: "); 
    Serial.println(lowByte(intValue)); // byte will print as regular int
    Serial.write("serWrite the low byte as char: "); 
    Serial.write(lowByte(intValue)); 
    Serial.write('\n'); // prints the low byte char
    // write always prints char value of byte or int to ser. mon.; but might send binary to other serial device ???
    Serial.print('\n');  // a new line
    delay(1000);
  } // end while
}

