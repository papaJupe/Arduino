/*
  Simb PinWake Demo shows plain wake and wake w/ specific callback
  adapted from Simb NonBLE/PinWake example
  SimbLily pin 3, plain wake, exits from ULPdelay, fast blinx x3, or delay
  times out and loop does slow blink x3; pin 11 wakes AND calls
  specific function to blink fast x2, ? does not exit ULPdelay ?

  Uses: specific callback fx, pinwake setups, ULPdelay call in loop and
  different LED response to plain wake and callback wake from different
  pins using pinWoke() conditionals, Serial.printf, resetPinWake
*/
byte led = 13;  // SimbLily's one lite

  // noisy pin/switch would need debouncing in the fx or before
int myPinCallback(uint32_t ulPin) // param not used b4, I use to print
{ // can't filter the interrupt itself, but may need to check the
  // pin again (debounce) here to see if it's valid before doing things
  // e.g. delay(20) if digRead(ulPin) == LOW
  
  // blink lite fast x2 on callback
  Serial.printf("callBack() by pin %d \n", ulPin);
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(500); 
  return 0;  // don't exit Simblee_ULPdelay 
}

void setup()
{   Serial.begin(9600);

  pinMode(led, OUTPUT);
  
  // pin event handled by exiting Simblee_ULPDelay, loop flashes lite
  pinMode(3, INPUT_PULLUP); // SimbLily has a pin 3
  Simblee_pinWake(3, LOW); //  ground makes low, turns ON

  // processing board pin event handled by myPinCallback
  pinMode(11, INPUT_PULLUP); // ground to turn ON
  Simblee_pinWakeCallback(11, LOW, myPinCallback);
 
}  // end setup

void loop() {

  // pin 3 wakes, flashes x3, doesn't Callback fx
  if (Simblee_pinWoke(3)) // 3 quick flash
  { Serial.println("pin 3 woke");
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite(led, LOW);
    delay(300);
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite(led, LOW);
    delay(300);
    digitalWrite(led, HIGH);
    delay(300);
    digitalWrite(led, LOW);
    delay(300);

    Simblee_resetPinWake(3);
  }
  else  // 3 slow flash (loop does when ULPdelay expires Q 2 sec)
  {
    Serial.println("timed out"); 
    delay(500); // to see if call back jumps here
    digitalWrite(led, HIGH);
    delay(1200);
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(1200);
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(1200);
    digitalWrite(led, LOW);
    delay(500);
  }
    Simblee_ULPDelay(SECONDS(2)); // delays loop & sleeps 2 sec or until
}  // end loop
