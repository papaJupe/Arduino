// BinaryDisplayFromInput -- experiment to input number (many ways possible), 
// display in binary lites; could get keybd, serial or switch input, use binary
// output other ways

// #define readMe 14  // provide arbitrary number < 7 or 16 if no other input source

byte readMe = 0;   // variable's 8 bits (max) can be read from switches or other input 
                // then used to set n LEDs on/off to display its binary format
byte pinIns[] = {2,3,4};        // input pins for 3 bit binary switch
byte pinOuts[] = {9,10,11,12};  //  these are [0,1,2,n] output pins to LEDs
                               //  for 1s,2s,4s,8s; my 3 bit switch only goes 0-7
                               
void setup()    //  code here runs once:
{
  Serial.begin(9600);
  
  pinMode(2,INPUT);       // digital pins that will read inputs
  pinMode(3,INPUT);       // config more if you have more inputs
  pinMode(4,INPUT);      // can also use INPUT_PULLUP here, without next stuff
  
  digitalWrite(2, HIGH);      // enable their internal pullup resistors
  digitalWrite(3, HIGH);      // pin 2 to low bit, pin 3 to 2's bit, etc
  digitalWrite(4, HIGH);
  
  pinMode(9,OUTPUT);     // pins that will light LEDs, 9 = low bit, etc
  pinMode(10,OUTPUT);    // could also use loop to config I/O pins from arrays
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
}

void loop()   // loop runs repeatedly
{
      // set bits of byte readMe from inputs, e.g. rotary 3 bit switch
  for (byte i=0; i < sizeof(pinIns); i++)
  {
   boolean bitOn = digitalRead(pinIns[i]);  // switch pulls the pin low if it's closed
   if (bitOn  == 0)                        // so if low set this bit of readMe to 1
      bitWrite(readMe, i, 1);
   else
      bitWrite(readMe, i, 0);
  }
 
 
             // display the var (readMe) on n LEDs in binary code, low endian I/O
  for (byte i=0; i < sizeof(pinOuts); i++)  // or ; "i < sizeof(pinOuts);"
  { 
    byte pin = pinOuts[i];
    if (bitRead(readMe, i) == 1) // if bit i of readMe is 1, make respective
                                 // binary output pin high  
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
  } // optionally view number on ser. mon.
  Serial.print("readMe is now: "), Serial.println(readMe);
  delay(500);
} // end loop
