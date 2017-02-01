/* 
serialReadWriteAM3 -- AM revision of PY's for Ardu <-> SmartDesktop comm;
    digi & anal inputs & reading ser bool input to control leds; uses readBytes()
    to put full input string into char[], then for loop to read it, turn lites on/off
    my v. 2 added code for strcat of float var;  
    v. 3 tests various ways to re-init/clear the 'out' array each loop
    Uses: char/string array, boolean input/output, Serial.readBytes
    dtostrf (vFlt,4,3,vStr) convert float to string,
    sprintf(tempstr,"%s;",vStr);   // put numbers / string into ch array buffer
    strcat(out,tempstr)          // add buffer string to array
*/
    // global vars used in setup and loop
byte numBoolInput=3;   // how many digital switches
byte numAnalInput=2;   // how many analog inputs
byte numLites=5;       // how many LEDs controlled by serial input

char out[16],in[16],tempstr[16];  // containers for in/out strings, no values assigned (all null?)
    // is it a problem that later I add strings of one char to these arrays? 
    // no, they seem to be stored as indiv char in array anyway
    
void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(500);   // 500 mS timeout for Serial functions, how does it work?
        // configure digital pins 2-13 (0,1 are unusable); analog pins (14-19) default to INPUT
        // so could go up to i<20 in next for loop; but still would need INPUT_PULLUP for dig pins;
  for (byte i=2;i<14;i++)   //  = range of digital I/O pins for Uno
    {               // so if 2 input switches, they will connect to 2,3 (and ground)
      if (i<=numBoolInput+1)   // if you have 2 switches, numBoolInput=2, so you want to 
                     // configure 2 and 3 to be input
         pinMode(i, INPUT_PULLUP);       // config input pins and enable internal pullup
           
      else pinMode(i, OUTPUT);   // outputs for LEDs
    }   // end for
        
    // digitalWrite(16, HIGH);  // if using A2 as digital input or could use INPUT_PULLUP
    
}   //end setup

void loop()
{   while (!Serial.available());  //  stops here, then typing any char should run loop once
    delay(10);  // may help with flakiness of Serial
    // Serial.read(); // clears buffer of one char, so next loop stops at while; if you don't care about value
    // out[2]=0;     // zeros (null) 2 (3rd) element, leaves other elements intact from prev. loop
    // char out[14];  // just re-declar leaves what's there in memory 
    // char out[16] = {'0'};   // re-declar puts ch 0 in [0], null others, sizeof() = 16 bytes 
    char out[16] = {0};   // re-declar puts #0 (null) in all slots, sizeof() = 16 bytes
                          // whether the slots are filled or null, strlen = how many char in it
    // int out[5] = {2};   // new declar puts 2 in [0], null 0 in others; each int slot = 2 byte
                          // so sizeof() is 10, whether the slots are filled, 0, or null
                          
   strcat(out,"B");   // appends B to out (should be empty, all 0s (null)); works if array is char type
        // so a string is added as indiv chars: "Bx" adds B to 0, x to 1 slot; strcat can't use (out,'B')
  Serial.println("--just added B to out: ");
  
  for (byte i = 0; i < sizeof(out); i++) // prints what's in out[] after adding B above (debug array clear)
    {
    Serial.print(i);  // ,DEC makes it a number, but not needed here
    Serial.print(" indx = ");
       // in this line, print (+/- the ,DEC formatter) prints an int as #, char as itself; 
       //.write always prints char equiv in ser mon, B if 66, blank if 0
       // with ,DEC format below, char[] elements print as their ASCII #, 0 for null, etc
    Serial.print(out[i],DEC),Serial.print('\n');   //newline 
    }
    
  for(byte i=4; i>=2; i--)  // to read dig->bin switch correct order, D4->D2, hi -> low bit
     // normally -- for (int i=3;i<=numBoolInput+2;i++)  // starting with pin D3 going up
  {                     // read state and add 0 or 1 to out (held only B at first)
    if (digitalRead(i)==HIGH)  // pullup keeps high, so switch is open
        strcat(out,"0");   // appends string of one char, '0' doesn't work
      // out[i]= 0;  // or '0' ?
    else                // switch is closed so
       strcat(out,"1"); // put a one in array; does fx keep a \0 at end of array?
       // may have \0 from intializ and not filling completely ?
      //  out[i]=1;
  }  // end for
  
  Serial.println("now adding dig reads 4,3,2");  // reprint after adding 3 chars above
  for (byte i = 0; i < sizeof(out); i++)
    {  
    Serial.print(i);  // index of the element
    Serial.print(" = ");  // prints the char unless you use ,DEC, then the ASCII value
    Serial.print(out[i]),Serial.print('\n');  // .write to ser mon shows char equiv of the 
                                          // byte / char it gets
    }
    
  
  strcat(out,";A");  // append ;A to out
  for (int i=2;i<numAnalInput+2;i++)   // start with A2
  {         // put int values of Anal pins into tempstr (a global char array)
    sprintf(tempstr,"%d;",analogRead(i));   // so ints are now char's in tempstr array?
    strcat(out,tempstr);      // -- then appends it to out; out now holds bunch of chars, ending in '\0' ?
  } 
//       // above is fine for int value, not for float; but this works: see v. AM2 for use
//  float vFlt = 4.77 *((float)analogRead(0)/1023);  // need the cast from an int var
//  char vStr[] = "0";    // make the buffer array; but put a string in it ???, how does it enlarge?
//            // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER);
//  dtostrf (vFlt,4,3,vStr);  // float now held as string ?
//  sprintf(tempstr,"%s;",vStr);   // put string into tempstr
//  strcat(out,tempstr);           // -- and appends it to out

              // print at more reasonable pace
//  static byte counter = 20;   // set it once, to print on first loop
//  if(counter==20)              // then print every 20 loops
//    {
//      Serial.print("out[] holds: "), Serial.println(out);    // prints combined digital and analog data as char's
//                               // sends out data, but doesn't affect Ardu lights
//        counter = 0;     // reset after each print
//    }
//  counter++;

                     // .readBytes(buffer,length) puts (length) bytes into (buffer) ch array
  Serial.readBytes(in,Serial.available());  // reads all bytes in Serial buffer, 
  if (strlen(in)>0)                // puts them into 'in' global char array
    {    // incoming string assumed to look like: D01100 or more, 
       // but only char elements [1-5] turn lites ON / OFF
      for (int i=1;i<strlen(in);i++)       // loop thru incoming bytes starting with [1] element
       {
      if (in[i]=='0')     // turn these off
        {
        digitalWrite(i+8,LOW);    // array element 1 writes to D9, etc
        }
     // for testing of array clearing -- if these chars cleared from out array on next loop
//       else if (in[i] == '4')  // 4 anywhere in input puts x,y in array
//        {out[5] = 'x'; out[6] = 'y'; }
      else
        {             // and any non-zero turns others on
        digitalWrite(i+8,HIGH);  // 5 values write to pins D9-D13
        }
       }   // end for
   }  // end if

    Serial.print("out[] holds: "), Serial.println(out);
    Serial.print ("sizeof out is "), Serial.println(sizeof(out)); // size of array in bytes, not what's in it
    Serial.print ("strlen out is "), Serial.println(strlen(out));
 // delay(2000);

}  // end loop
