/*
  blink3 sketch
  demonstrates calling a function with a parameter and returning a value
  uses the same wiring as the pull-up sketch from Recipe 5.2
  The LED flashes when the program starts
  and stops when a switch connected to pin 2 is pressed
  the program prints the number of times the LED flashes.
 */

const int ledPin = 13;          // output pin for the LED
const int inputPin = 2;         // input pin for the switch

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin,HIGH); // use internal pull-up resistor (Recipe 5.2)
  Serial.begin(9600);
}

void loop(){
  Serial.println("Press and hold the switch to stop blinking");
  int count = blink3(250); // blink the LED 250ms on and 250ms off
  Serial.print("The number of times the switch blinked was ");
  Serial.println(count);
}

// blink an LED using the given delay period
// return the number of times the LED flashed
int blink3(int period)
{
  int result = 0;
  int switchVal = HIGH; //with pull-ups, this will be high when switch is up

  while(switchVal == HIGH)  // repeat this loop until switch is pressed
                            // (it will go low when pressed)
  {
    digitalWrite(ledPin,HIGH);
    delay(period);
    digitalWrite(ledPin,LOW);
    delay(period);
    result = result + 1; // increment the count
    switchVal = digitalRead(inputPin);  // read input value
  }
  // here when switchVal is no longer HIGH because the switch is pressed
  return result;  // this value will be returned
}
      
    
