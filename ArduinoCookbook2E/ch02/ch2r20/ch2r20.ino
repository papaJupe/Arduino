/*
 * bits sketch (cookbook Ch 2)
 * demonstrates bitwise operators and malfunction of .print in Ardu C
 */

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("3 & 1 equals "); // bitwise And 3 and 1
  Serial.print(3 & 1);         // print the result
  Serial.print(" decimal, or in binary: ");
  Serial.println(3 & 1 , BIN);   // print the binary 
                                 // representation of the result

  Serial.print("3 | 1 equals "); // bitwise Or 3 and 1
  Serial.print(3 | 1 );
  Serial.print(" decimal, or in binary: ");
  Serial.println(3 | 1 , BIN);   // print the binary 
                                 // representation of the result

  Serial.print("3 ^ 1 equals "); // bitwise exclusive or 3 and 1
  Serial.print(3 ^ 1);
  Serial.print(" decimal, or in binary: ");
  Serial.println(3 ^ 1 , BIN);   // print the binary 
                                 // representation of the result


  byte byteVal = 1;
  int intVal = 1;
Serial.println("bytes in intVal before neg " + String(sizeof((intVal)))); // =2
  byteVal = ~byteVal;  // do the bitwise negate
  intVal = ~intVal;
Serial.println("bytes in intVal post neg " + String(sizeof((intVal)))); // =2

  Serial.print("~byteVal (1) equals "); // bitwise negate an 8 bit value
  Serial.println(byteVal, BIN);  // print the binary 
                                 // representation of the result
  Serial.print("~intVal (1) equals "); // bitwise negate a 16 bit value
  Serial.println(intVal, BIN);   // print the binary representation of
            //  the result. BUT prints 32 bits not the 16 it should!
            // some weird malfunction of .print ??? carryover from C

  delay(10000);
}
