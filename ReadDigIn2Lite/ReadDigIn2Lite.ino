/*
  Read_Dig_to_Lite -  reads one hi/lo input and LED displays on/off 
  -- used to test dig IR sensors et. al.
    Uses: device power from 5v, pinMode, digitalRead/Write
*/


#define DigIn 7     // digital pin IN
#define LiteOut 3  // LED output

void setup()
{
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);

  pinMode(DigIn, INPUT_PULLUP);
  pinMode(LiteOut, OUTPUT);

  // pins supply neg indicator LED (2/3) & power to sensor (7)
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);

  
}  // end setup

void loop()
{
   digitalWrite(LiteOut, !digitalRead(DigIn));  // if sensor/switch pulls input low, turn ON the LED
   // digitalWrite(LiteOut, digitalRead(DigIn));  // lite on if dig input goes high, so does LED output

  
  delay (50);   // N loops / second
}  // end loop
