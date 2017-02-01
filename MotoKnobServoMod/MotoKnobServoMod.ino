// Originally controlled servo position using a potentiometer 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>
// Mead's mod to control a motor speed within narrow range set by values in map function
//  -- assuming 90 deg for servo = 1500 uS pulse width = motor stopped; 
// below that, motor runs back; above, it runs forward

#include <Servo.h> 
 
Servo mymoto;  // creates servo object to control a motor 
 
int potpin = A2;  // analog pin to read the potentiometer voltage
int valr;    //  variable reads the value from the analog pin 
int val;     // mapped int to write a variable width pulse to output pin

void setup() 
{ 
   pinMode(potpin, INPUT);   // put pot ends to 5v and grnd, center to A2
   pinMode(A3,OUTPUT);
   pinMode(A1,OUTPUT);
   digitalWrite(A3,HIGH);
   digitalWrite(A1,LOW);
   //Serial.begin(9600);   //need for printout of values
   mymoto.attach(2);     // attaches pin 9 to control the servo object 
} 
 
void loop() 
{ 
  valr = analogRead(potpin);            // reads the volts from the potentiometer (between 0 and 1023) 
  val = map(valr, 0, 1023, 0, 180);     // scale it to degrees for servo (full value between 0 and 180 or partial range) 
                                       // servo position center = 90, pulse = 1500 uS, motor still
                                      //  70-110 gives range of 1280 - 1680 uS
  mymoto.write(val);                  // sets the servo position (motor speed) according to the scaled value 
//  if (Serial.available())       //type any letter to get printout of vals, mostly for debugging
//   { Serial.read();  // serves no purpose except to clear serial buffer
//     Serial.print(valr); Serial.print("  "); Serial.println(val);
//   }   //end if
  delay(100);                           // waits for the servo to get there 
} 


