/* SeriPrintWrite to show output of ser. mon. using .write vs .print for a byte argument; 
 but both show same char output in ser. mon. when the argument is a string;
 .write sends binary byte to serial port, but hard to see with ser.mon. or terminal
 (shows same output) as they both show the binary input as char's
 Uses: serial.write, serial.print
 */

void setup()   // just prints stuff once
{

  Serial.begin(9600), Serial.write("\r\n");  // \r's needed for unix 'screen', ser. mon. doesn't care
  Serial.write("given a number, 123, .write sends as 1 byte to ser. mon.\r\n  which prints it as char equivalent of that byte, L curly brace\r\n");   
  // 123 = ascii for { which is printed; .write sends string var as bytes, also shown as char's by ser.mon.
  byte size = Serial.write(123);
  Serial.write(13), Serial.write(10);  // ascii for \r, \n
  Serial.print(size),Serial.println("  (size var) is the # of bytes sent");

  Serial.println();
  for (byte n =99;n<110; n++)     // 99 is c, 109 is m; if you put "char n = " here
                              // you always see the char printed -- OK up to 127 
                              // then if >127 it gives weird output, and never stops looping
                              // n (cast as int) goes up but doesn't stop for() action;
                              // char type stops at 127 then resets to -128
    // using int n or byte n (up to 255) = it stops OK, but hi output not regular ASCII, maybe UTF8
  {
    // this prints n's (int value of the byte and as char)
    Serial.print(n), Serial.println("  Ser.prints the numeric ascii of the byte as chars"); 
    Serial.write(n), Serial.write("  w/ Ser.write -- ser. mon. shows the ascii char of the byte\r");
    Serial.write(10); // newline

    delay(200);
  }   // end for

} // end setup

void loop() 
{
  // put your main code here, to run repeatedly:

}






