/*
  Read_Dig_to_Lite -  reads one hi/lo input to D4 and LED displays on/off at D12 
  -- used to test dig IR sensors et. al.
    Uses: device power from pins
*/


#define DigIn 4     // digital pin IN
#define LiteOut 12  // LED output

void setup()
{
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);

  pinMode(DigIn, INPUT_PULLUP);
  pinMode(LiteOut, OUTPUT);

  // these supply neg and 5v to sensor and lite neg
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  
}  // end setup

void loop()
{
  // digitalWrite(LiteOut, !digitalRead(DigIn));  // if sensor/switch pulls input low, turn ON the LED
   digitalWrite(LiteOut, digitalRead(DigIn));  // if dig input goes high, so does LED output

  
  delay (100);   // ten loops / second
}  // end loop
