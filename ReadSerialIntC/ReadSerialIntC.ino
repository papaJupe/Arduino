/*
  ReadSerialIntC -- takes mixed char input from keybd, extracts 1st int,
  converts from C to F temp; orig used Serial.parseInt which is weak at
  parsing int mixed with other chars, so ReadSerIntB puts incoming
  digits into a String object then Str.toInt() to convert. This v. C
  puts input bytes into char array, extracts first group of digits
  from it into another array then uses atoi (or conv to String then
  .toInt) to make the int; reinit or memset clears array each loop;
  handles neg # input OK, may need to limit input to 9 char

  Uses: Ser.readBytes, atoi, isDigit, strlen, memset, string.toInt, strcpy

  --  byte array to int using String.toInt conv
  byte byteArray[5];
  // 2nd arg must be string or char, 1st an array ptr?
  strcpy((char *)byteArray,"0123"); // below I just use next line, w/o cast
  String myString = String((char *)byteArray);  // ? need cast if already char[], No
*/

int c = 0;
int f;
boolean neg; // T if (-) precedes a digit
char inByt[10];  // byte array to hold mixed incoming chars, up to 9-10
char inDig[10];  // char array to hold digits extracted from inByt

int cenToFare(int c)
{
  return (c * 9 / 5 + 32);
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{ // each loop asks for new input to convert -- should parse int from mixed chars
  char inByt[10] = {}; // reinit here clears array, all null now ?
  char inDig[10] = {0}; // blank or 0 do same thing
  neg = false;
  Serial.println("enter C. temp to convert: < 10 chars");  // like '20 deg C'

  while (!Serial.available()); // while no serial input, hold the action
  delay(30);  //flaky reading of chars unless some delay
  boolean gotOne = false; // to track if some incoming digit detected already
  int j = 0; // index for inDig, need reinit each loop to process new # input

  if (Serial.available() > 0)
  {
    // int c = Serial.parseInt();  // keybd entry of 2-3 digits of temp in C. OK
    // parseInt OK for numbers and letter+number, gets fooled by letter alone or number+letter

    // .readBytes(buffer,length) puts (length) bytes into (buffer) array
    Serial.readBytes(inByt, Serial.available()); // ALL incoming bytes --> inByt char array
    Serial.print("inByt using .print ");
    Serial.print(inByt);
    Serial.write('\n');
    while (Serial.read() != -1); // clear the input buffer (should be already)
  }  //end if serial.avail  -- it's all in ch array now

  if (strlen(inByt) > 0)
  { // incoming string can be mix of digits & char's; we want just 1st valid int
    // Serial.print("strlen of inByt "); Serial.println(strlen(inByt)); // how many char in array
    for (int i = 0; i < strlen(inByt); i++) // loop thru incoming bytes starting with 0 element
    {
      // test if it's a (-) followed immediately by digit & no digits yet
      if (inByt[i] == '-' && !gotOne && isDigit(inByt[i + 1])) neg = true;
      if (isDigit(inByt[i]))     // add it to clean array of digits
      {
        inDig[j] = inByt[i];
        gotOne = true;  // T if you've got at least one digit from input
        Serial.print("got a dig ");
        Serial.println(inDig[j]);
        Serial.println(inDig);
        j++;
      }  // end if isDigit
      // if you get a non-digit after digit(s), ignore and break (after printing it);
      else if (gotOne)
      {
        Serial.print("got a non-dig ");
        Serial.println(inByt[i]);
        break;   // out of the for loop
      } // end else
    } // end for
  }  //end if some input --> inByt


  // convert the array to int if not empty; strlen works, maybe
  // ? because chars in array end with null \0, look like string
  if (strlen(inDig) != 0)
    //    {
    //      c = atoi(inDig);     // conv digit char array to int
    //      int nowInF = cenToFare(c);    // call function with temp C
    //      Serial.print("F temp is ");
    //      Serial.println(nowInF);
    //      c = 0;
    //    }
  {
    // or can convert filtered char array to String and use str.toInt
    String myString = String(inDig);
    c = myString.toInt();
    // if neg, mult x -1
    if (neg) c = c * (-1);
    int nowInF = cenToFare(c);    // call function with temp C param
    Serial.print("F temp is ");
    Serial.println(nowInF); Serial.println();
    c = 0;
    j = 0;  // reset the index for inDig
  }
  else Serial.println("invalid entry"); // no digits seen --> empty string


  //   clear both arrays by reinit (seems OK above); memset works too
  //   char inByt[10]= {};
  //   char inDig[10]= {};

  // void * memset ( void * ptr, int value, size_t num );
  //  memset(inByt, 0, 10);  // since these are char arrays, is this null or '0'? I think null
  //  memset(inDig, 0, 10);

}  // end loop



