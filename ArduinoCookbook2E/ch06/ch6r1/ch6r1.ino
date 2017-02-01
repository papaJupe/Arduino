/*
tilt sketch

  a tilt sensor attached to pin 2,
  lights one of the LEDs connected to pins 11 and 12
  depending on which way the sensor is tilted
*/

const int tiltSensorPin = 2;             //pin the tilt sensor is connected to
const int firstLEDPin = 11;              //pin for one LED
const int secondLEDPin = 12;             //pin for the other

void setup()
{
  pinMode (tiltSensorPin, INPUT);        //the code will read this pin
  digitalWrite (tiltSensorPin, HIGH);    // and use a pull-up resistor

  pinMode (firstLEDPin, OUTPUT);         //the code will control this pin
  pinMode (secondLEDPin, OUTPUT);        //and this one
}

void loop()
{
  if (digitalRead(tiltSensorPin)){       //check if the pin is high
    digitalWrite(firstLEDPin, HIGH);     //if it is high turn on firstLED
    digitalWrite(secondLEDPin, LOW);     //and turn off secondLED
  }
  else{                                  //if it isn't
    digitalWrite(firstLEDPin, LOW);      //do the opposite
    digitalWrite(secondLEDPin, HIGH);
  }
}
      
    
