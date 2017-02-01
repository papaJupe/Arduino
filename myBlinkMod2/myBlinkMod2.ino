/*
 Blink
 Turns on an LED on for (blinkDelay), then off for (blinkDelay) repeatedly.
 I modified to accept input from serial monitor and show what's running
 Mod2 adds EEPROM persistent storage of delay value between restarts
 */
#include <EEPROM.h>   // for persistent storage, up to 1 kb

// Pin 13 has an LED installed on most Arduino boards;
// give it a name:
int led = 13;
int blinkDelay = 1500;
int addr = 2; // your persistent memory addr, start at 0 (or other loc you choose)

// the setup code runs once when you power up or press reset or break serial connection:

void setup() 
{                
  // initialize digital pin __ as an output
    pinMode(led, OUTPUT);
    Serial.begin(9600);
//for(int i =0; i <=1023; i++)  // if misbehaving, storing -1 ???, clear it
//      EEPROM.write(i,0);
//  Serial.println("cleared eeprom");  
 //if there's a number stored in EEPROM at addr, use it, else default stays put
  if ((EEPROM.read(addr) > 0) || (EEPROM.read(addr+1) > 0))
    {
        byte high = EEPROM.read(addr);
        byte low = EEPROM.read(addr+1);
   
        blinkDelay = (high << 8) + low;
    }
  Serial.print("blink delay is ");
  Serial.println(blinkDelay);
}  // end setup

// the loop runs over and over:
void loop() 
{
  if (Serial.available())   // new int entered, so reset delay and put it in EEPROM
  {   
    blinkDelay = Serial.parseInt(); // reads characters as numbers, empties serial buffer
    delay(50);
    Serial.print("delay is reset to "); // print new value
    Serial.println(blinkDelay);  // when you open new ser. mon., break in serial comm causes Reset
                              // but you store the new value, so it should stick thru Reset 
    EEPROM.write(addr, highByte(blinkDelay));
    EEPROM.write((addr+1), lowByte(blinkDelay));
    
//    byte high = EEPROM.read(addr);
//    byte low = EEPROM.read(addr+1);
//    int x = (high << 8) + low;
//    Serial.print("held in memory: "); Serial.println(x);
  }  // end if serial avail.
  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blinkDelay);               // wait for a sec
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(blinkDelay);               // wait for a sec
}  // end loop

