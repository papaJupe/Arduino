/*
  ReadSerialIntB -- takes number input from keybd, converts to int, converts from C to F temp
  orig used Serial.parseInt which is incapable of parsing int mixed with other chars, so
  ReadSerIntB puts incoming digits one by one, into a String object then StrObj.toInt()
  to convert. works with mixed digits, letters, mod to catch initial (-) for C temps < 0 
  
  Version C extracts first group of digits from the serial input into an array
  then atoi to make the int, still fails w/ minus temps

  Uses: String object, adding char's to it, Serial.read, serial.toInt()
*/

int c = 0;
int f;
String inString;  // String object to hold incoming digits

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println("enter C. temp to convert");
  inString = "";
  while (!Serial.available()); // while no serial input, hold the action
  delay(20);  // NOT optional: flaky reading of chars and loop malfunction unless some delay
  boolean gotOne = false; // will track if some incoming digits detected already in loop
  boolean neg = false;
  while (Serial.available() > 0)
  {
    // int c = Serial.parseInt();  // keybd entry of 2-3 digits of temp in C.
    // parseInt OK for numbers and letter+number; but fooled by letter alone or number+letter

    int inChar = Serial.read();  // int,byte,char work here
    // test if it's a (-)
    if (inChar == '-' && !gotOne) neg = true;  // got a (-) and no digits yet
    if (isDigit(inChar))
    {
      // cast the incoming byte to a char and add it to the String object:
      inString += (char)inChar;
      gotOne = true;  // you've got at least one digit from input
    }
    // if you get a non-digit after some digit(s), ignore and break from while loop;
    else if (gotOne)  // need to flush Ser buffer and flush() does not flush, this does
    {
      while (Serial.read() != -1); // read incoming buffer until empty
      break;
    }    // w/o this else block, digits coming after a letter also get added to String
  }  //end while serial.avail

  // convert the String to int if not empty
  if (inString != "")
  {
    c = inString.toInt();  // ignores non digit like (-)
    if (neg)  c =  (-1) * c;
    int nowInF = cenToFare(c);    // call function with temp C
    Serial.print("F temp is ");
    Serial.println(nowInF);
  }
  else Serial.println("invalid entry"); // no digits seen --> empty string
}  // end loop

int cenToFare(int c)
{
  return (c * 9 / 5 + 32);
}

