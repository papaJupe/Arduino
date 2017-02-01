/* 
 SerialReadWriteAM2   -- AM 2nd revision of PY's for Ardu <-> SmartDesktop comm
 v.2 adds float output & Streaming lib to print C++ style using insertions;
 reads digital and analog pin values, prints to serial output
 Uses: Streaming lib, boolean I/O,  strcat, sprintf, dtostrf, Serial.readBytes
 ? still have basic questions about what char arrays contain after strcat, sprintf;
 coded here for 2 anal inputs on A3,4 and 3 digital switches on D2,3,4 & 5 leds on 9-13
 */

#include <Streaming.h>

int numBoolInput=3;   //  how many digital switches
int numAnalInput=2;   // how many analog inputs
int numLites=5;       // how many LEDs controlled by serial input or local pins

char out[20],in[20],tempstr[20]; // char array containers for in/out strings

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(500);   // 500 mS timeout for Serial functions, how does it work?
  for (int i=2;i<14;i++)    // configure digital pins 2-13; analog pins default to INPUT
  {               // so if 2 dig input switches, they must connect to 2&3 (and ground)
    if (i<numBoolInput+2)   // if you have 2 switches, numBoolInput=2, so you want to 
      // configure 2 and 3 (numBoolInput+1 will be last i to config pin i)
    {   
      pinMode(i, INPUT);       // config input pins and enable internal pullup
      digitalWrite(i, HIGH);  // or just use INPUT_PULLUP
    }  
    else pinMode(i, OUTPUT);   // outputs for LEDs

  }   // end for
  // digitalWrite(15, HIGH);  // my A1 switches between 0 and 5 v. using ground

}   //end setup

void loop()
{ 
  float vFlt; // declare here to use in multiple blocks in loop
  // strange rapid printing if I don't use one of these to re-init out, ?? why
  // various re-init options tested in v. 3; this works properly to clear array
  char out[20] = {
    0  };   // re-declar/init, now a local array, all null
  strcat(out,"B");   // appends to (should be empty) array at index 0
  for(int i=4; i>1; i--)  // to read dig-binary switch correct order, D4->D2
    // was --  for (int i=3;i<=numBoolInput+2;i++)  // starting with pin D3 going up
  {          // read state and add 0 or 1 to out (empty except for B)
    if (digitalRead(i)==HIGH)  // pullup is high, so switch is open
    {
      strcat(out,"0");   // appends string of one char, can't use 'ch'
    }  // end if
    else                       // switch is closed
    {
      strcat(out,"1"); 
    } // end else
  }  // end for

  strcat(out,";A");  // append ;A to out, to signal start of Anal values
  //  for (int i=0;i<numAnalInput;i++)   // get all anal values 0-5, put in out
  for (int i=2;i<numAnalInput+2;i++)   // to want vals from A3,4 only
  {
    //    sprintf(tempstr,"%d;",analogRead(i));   // put int values of Anal pins__ plus ; into tempstr
    //    strcat(out,tempstr);               // (a char array); then appends it to out array
    //  } // end for

      // above is fine for int value, not for float; but this works (& adds 2K to sketch size):
    vFlt = 4.77 *((float)analogRead(i)/1023);  // need the cast from an int var to divide

    //if you want to add the float to out
    char vStr[6] = {
      0    };
    ;    // make a buffer array big enough to hold float as digits, all null
    // convert the float voltage to a string with dtostrf
    dtostrf (vFlt,5,3,vStr);    // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER);

    sprintf(tempstr,"%s;",vStr);   // put buff string plus ; into tempstr
    strcat(out,tempstr);           // -- and append it to out  
  } // end for

    static byte counter = 20;    // set it once
  if(counter==20)              // only print every 20 loops
  {         
    Serial<<"outstr= " << out << '\n';  // OK to insert single char too
    // with <Streaming.h> included, can print the float var by insertion directly
    // tho this adds ~1.5Kb to your sketch size;
    Serial << "and flot2 = " << vFlt << " volts" << endl; 
    // float print defaults to 2 digits; for 3 digit precision use _FLOAT(vFlt,3)  
    // other formatting prefixes for numbers: _BIN, _HEX, _DEC defined in the streaming.h

    // for regular printing use this               
    // Serial.println(out);    // prints combined digital and analog data as char's 
    // sends out data, but doesn't affect Ardu leds, readBytes function below does
    counter = 0;
  } // end if
  counter++;  // increm each loop to calm down printing

  // to control LEDs with some Serial input:
  // .readBytes(buffer,length) puts (length) incoming bytes into 'in' (buffer) array
  //  Serial.readBytes(in,Serial.available());  // reads all bytes from Serial buffer, 
  //  if (strlen(in)>0)                         // puts them into 'in' char array
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


