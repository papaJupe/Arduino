/* 
serialReadWriteAM1 -- AM revision of PY's for Ardu <-> SmartDesktop comm;
    digi and anal inputs & reading ser bool input to display; my 2nd version added 
    code for strcat of float var; many questions about array use here
    Uses: char/string array, boolean input/output, Serial.readBytes
    dtostrf (vFlt,4,3,vStr) convert float to string,
    sprintf(tempstr,"%s;",vStr);   // put string into buffer
    strcat(out,tempstr)  // add buffer string to array
*/
    // global vars used in setup and loop
int numBoolInput=3;    //  how many digital switches
int numAnalInput=2;   // how many analog inputs
int numLites=5;          // how many LEDs controlled by serial input

char out[20],in[20],tempstr[20];  // containers for in/out strings
    // is it a problem that later I add strings of one char to these arrays? Are they stored as char or strings?
    
void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(500);   // 500 mS timeout for Serial functions, how does it work?
        // configure digital pins 3-13 (0-1 unusable); analog pins (14-18) default to INPUT
        // but still would need INPUT_PULLUP; could go up to i<19 in next for loop:
  for (int i=3;i<14;i++)   
    {               // so if 2 input switches, they will connect to 3,4 (and ground)
      if (i<=numBoolInput+1)   // if you have 2 switches, numBoolInput=2, so you want to 
                     // configure 3 and 4 to be input
    {   
     pinMode(i, INPUT_PULLUP);       // config input pins and enable internal pullup
     }  
    else pinMode(i, OUTPUT);   // outputs for LEDs
   
    }   // end for
        // analog 0 will be anal input
   // digitalWrite(15, HIGH);  // if using A1 as digital input or could use _PULLUP
    
}   //end setup

void loop()
{
    out[0]=0;      // zeros 1st element, but does it erase entire array ? as supposedly this does
   //  char out[20] = {0};   // looks like second declar but maybe C shortcut to empty array ?
  strcat(out,"B");   // appends B to out (should be empty now) array; array is char but this is string ???
  for(int i=4; i>2; i--)  // to read dig-bin switch correct order, D4->D3
     // normally --  for (int i=3;i<=numBoolInput+2;i++)  // starting with pin D3 going up
  {                     // read state and add 0 or 1 to out (holds only B at first)
    if (digitalRead(i)==HIGH)  // pullup keeps high, so switch is open
     {
      strcat(out,"0");   // appends string of one char
      }  // end if
    else                       // switch is closed
      {
      strcat(out,"1"); // put a one in array
      } // end else
  }  // end for
  
  strcat(out,";A");  // append ;A to out
  for (int i=0;i<numAnalInput;i++)
  {                              // put int values of Anal pins 0,1 or more into tempstr (a char array)
    sprintf(tempstr,"%d;",analogRead(i));   // so are they now char strings ? in tempstr
    strcat(out,tempstr);                         // -- then appends it to out; does out now hold strings ?
  } 
       // above is fine for int value, not for float; but this works: see v. AM2 for use
//  float vFlt = 4.77 *((float)analogRead(0)/1023);  // need the cast from an int var
//  char vStr[] = "0";    // make the buffer array; but put a string in it ???
//                     // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER);
//  dtostrf (vFlt,4,3,vStr);  // float now held as string ?
//  sprintf(tempstr,"%s;",vStr);   // put string into tempstr
//  strcat(out,tempstr);           // -- and appends it to out

              // print at more reasonable pace
  static byte counter = 20;   // set it once, to print on first loop
  if(counter==20)              // then print every 20 loops
    {
      Serial.println(out);    // prints combined digital and analog data as char's
                               // sends out data, but doesn't affect Ardu lights
        counter = 0;     // reset after each print
    }
  counter++;
                // .readBytes(buffer,length) puts (length) bytes into (buffer) array
  Serial.readBytes(in,Serial.available());  // reads all bytes in Serial buffer, 
  if (strlen(in)>0)                // puts them into 'in' char array
    {    // incoming string assumed to look like: D01100 or more, but only char elements [1-5] affect lites
       for (int i=1;i<strlen(in);i++)       // loop thru incoming bytes starting with [1] element
       {
      if (in[i]=='0')     // turn these off
        {
        digitalWrite(i+8,LOW);    // array element 1 writes to D9, etc
        }
      else
        {             // and any non-zero turns others on
        digitalWrite(i+8,HIGH);  // 5 values write to pins D9-D13
        }
       }   // end for
   }  // end if
  
delay(100);  
  
}  // end loop
