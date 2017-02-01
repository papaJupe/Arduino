/*
 * SerialReceive sketch  -- read more than 1 digit from serial, needs Newline sent from ser. mon.
 * Blink the LED at rate inversely proportional to the received digit value
-- it reverts to default delay when ser. mon. window is closed -- causes reset ?
*/
const int ledPin = 13;  // pin the LED is connected to
int blinkDelay=100;     // default blink delay stored in this global variable
int value;

void setup()
{
  Serial.begin(9600); // Initialize serial port to send / receive at 9600 baud
  pinMode(ledPin, OUTPUT); // set this pin as output
  Serial.println("Newline must be enabled;");
  Serial.println(" -- then enter blink delay in ms.");
}

void loop()
{
  while (Serial.available()) // see if at least one character is available
  {
     char ch = Serial.read();    //automatically reads each char and deletes from buffer
     if(isDigit(ch) )                // is this an ascii digit between 0 and 9?
    {
       value = (value * 10) + (ch - '0'); //if yes, accumulate proper digital value as int
    } //end if                               this converts ascii value to int numeric value
    
     else if (ch == 10)  // it's the newline character, to mark end of input sequence 
                   // to mark seq. end, ser. mon. must send Newline with Send !!
    {     
      blinkDelay = value;  //reconstructed integer
      Serial.print("# received, blink delay = "); Serial.println(blinkDelay);
    } //end else
   } //end while Serial
   
  value = 0; // no new input, so reset value for next sequence to come in
  blink();   // only executes when no more Serial.available

} //end loop

void blink()        // blink the LED with rate inversely proportional to blinkDelay
{
  digitalWrite(ledPin,HIGH);
  delay(blinkDelay);
  digitalWrite(ledPin,LOW);
  delay(blinkDelay);
}
