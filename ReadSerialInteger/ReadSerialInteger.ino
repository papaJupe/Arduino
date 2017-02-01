/* 
ReadSerialInteger -- takes number input from keybd, converts to int, converts from C to F temp
Uses: Serial.parseInt
*/

int c = 0;
int f;
void setup() 
{
  Serial.begin(9600);
}

void loop() 
{   
  Serial.println("enter C. temp to convert");
  delay(20);  //flaky reading of chars unless some delay
  while(!Serial.available());  // while no serial input, hold the action
    
  delay(20);
  if (Serial.available())
   { 
     delay(20);  // flaky reading of chars unless some delay
     int c = Serial.parseInt();  // keybd entry of 2-3 digitd of temp in C.
     // parseInt works for numbers / letter+number; gets fooled by letter alone or number+letter
     int nowInF = cenToFare(c);    // call function with temp C 
     Serial.print("F temp is "); Serial.println(nowInF);
    }  //end if serial.avail
  
}  // end loop 
  
int cenToFare(int c)
 {
     return (c * 9/5 + 32);
 }
