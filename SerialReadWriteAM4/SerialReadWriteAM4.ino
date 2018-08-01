/* -- todo -- test if array is clearing by input various numbers, make parsing work w/ strcat etc
 SerialReadWrite4   -- AM 4th revision of PY's for Ardu <-> SmartDesktop comm
 v.2 adds float output & Streaming lib to print C++ style using insertions;
 v.4 to experiment with parsing an int from char array and String
 
 Uses: Streaming lib, boolean I/O,  strcat, sprintf, dtostrf, Serial.readBytes
 ? still have basic questions about what char arrays contain after strcat, sprintf;
 */

// #include <Streaming.h>

char out[20],in[20],tempstr[20]; // char array containers for in/out strings

void setup() 
{
  Serial.begin(9600);

}   //end setup

void loop()
{ 
  // must empty array somehow
  //out[0]=0;      // zeros 1st element (null), but also erases entire array, NO, ? as supposedly this does
  char out[20] = {0};  // re-declar&init, now a local array, all null?, would it work w/o reinit? no
  strcat(out,"B");   // appends to (should be empty now?) array
  strcat(out,"345");

  strcat(out,";A");  // append ;A to out, to signal start of Anal values
      // get V val from A_; for float this works (& adds 2K to sketch size):
    float vFlt = 4.88 *((float)analogRead(A0)/1023);  // need the cast from an int var to divide

    //if you want to add the float to out
    char vStr[6] = {0};    // make a buffer array big enough to hold float as digits; clear w/ vStr[6]={0}
    // convert the float to a string with dtostrf
    dtostrf (vFlt,5,3,vStr);    // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER);

    sprintf(tempstr,"%s;",vStr);   // put buff string plus ; into tempstr
    strcat(out,tempstr);  // -- and append it to out array

  static byte counter = 20;   // set it once
  if(counter==20)              // then only print every 20 loops
      {         
           // Serial << "outstr= " << out << '\n';  // OK to insert single char too
           // with <Streaming.h> included, can print the float var by insertion directly
           // tho this adds ~1.5Kb to your sketch size;
        // Serial << "and flot2 = " << vFlt << " volts" << endl; 
        // float print defaults to 2 digits; for 3 digit precision use _FLOAT(vFlt,3)  
    
        // for regular printing use this               
        Serial.println(out);    // prints combined digital and analog data as char's 
        // sends out data, but doesn't affect Ardu leds, as readBytes function below does
        counter = 0;
      } // end if
  counter++;  // increm each loop to calm down printing
  
    // .readBytes(buffer,length) puts (length) incoming bytes into 'in' (buffer) array
//  Serial.readBytes(in,Serial.available());  // reads all bytes in Serial buffer into buffer ==> into 'in' char array
//  if (strlen(in)>0)             
//  {    // incoming string assumed to look like: D01100 or longer 
//    //-- char elements [1 to (strlen-1)] are counted; only 1 - numLites affect lites
//    for (int i=1;i<strlen(in);i++)       //loop thru all incoming starting with element [1]
//    {
//      if (in[i]=='0')     // turn it off
//      {
//        digitalWrite(i+8,LOW);    // array element 1 writes to D9, etc
//      }
//      else
//      {             // any non-zero turns it on
//        digitalWrite(i+8,HIGH);  // 5 values write to pins D9-D13
//      }
//    }   // end for
//  }  // end if

  delay(200);  

}  // end loop


