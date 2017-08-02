// use .parseInt to read # from Ser. Mon.
// v. ReadSerialIntB/C for other soln
// this works with C temp < 0 by itself
// but doesn't handle mixed digits and letters

int c = 0;
int f;
const int MAXCH = 5;
char strVal[MAXCH + 1];
int index = 0;
 
void setup()
{
  Serial.begin(9600);
}
//  Ser.reads ASCII value of typed character and uses that for
// calculation, ie 0 read as 48 gives 118, so conv. needed or other soln

void loop()
{ 
    // memset(strVal, 0, 6); // should empty the array, all \0's
  Serial.println("enter C. temp to convert");

  while (!Serial.available()); // while no serial input, hold the action

  delay(20);  // NOT optional: flaky behaviour unless some delay
  boolean gotOne = false; // will track if any input digit detected already
  
   // code from Cookbook, needs newline in Ser Mon sends for some reason
//   while (Serial.available()) 
//     { 
//       delay(20);  //flaky reading of chars unless some delay
//  
//       // neither int or char work here w/o conversion, read as ASCII #
//      char ch = Serial.read();  // will be a 2-3 digit int for temp in C.
//      if ( index < MAXCH && isDigit(ch) )
//        {
//          strVal[index++] = ch; // 1st entered ch goes 1st in string
//        }
//      else
//        {
//          strVal[index] = 0; // term string w/ \0
//          // atoi conv string to #
//          c = atoi(strVal);
//          index = 0;      
//        } // end else
//      }  //end while serial.avail

  if (Serial.available())
   {
     delay(20);  //flaky reading of input unless some delay
     int c = Serial.parseInt();  // a 2-3 digit int for temp in C.
     int nowInF = cenToFar(c);    // call function with temp C
     Serial.print("F temp is "); Serial.println(nowInF);     
    }  //end if serial.avail


}  // end loop

int cenToFar(int c)   // works OK
{
  f = c * 9 / 5 + 32;
  return f;
}

// int cenToFar(int c)  // or this
// {
//     return (f = c * 9/5 + 32);  or simply:  return (c * 9/5 + 32)
// }
