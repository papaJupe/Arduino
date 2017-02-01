/* 
 Sensor Reader from Igoe MTT,ch.2,proj.2, added if block for handshaking w/ Proc.
 Context: Arduino
 
 Reads two analog inputs and two digital inputs and prints their values to ser. 
 mon. or ser. port -- to control pong game in Processing. Type any char+ret to
 trigger printing a string of sensor values, or send any byte from Proc.
 
 Connections:  
 analog sensor(s) on 2 analog input pins (or pot simulates them)
 2 momentary switches on digital I/O pins
 
 */

const int leftSensor = 3;    // analog input L
const int rightSensor = 4;   // analog input R
const int resetButton = 15;   // digital input for reset button 15=A1
const int serveButton = 14;   // digital input for serve button 14=A0 

int leftValue = 0;           // reading from the left 
int rightValue = 0;          // reading from the right
bool reset = 0;               // reading from the reset button, bool = boolean
boolean serve = 0;            // reading from the serve button


void setup() 
{
  // configure the serial connection:
  Serial.begin(9600);  // needs to be same as Proc's setting for it to read serial port
  // configure the digital inputs to any 2 pins (> D0 or D1)
  pinMode(resetButton, INPUT_PULLUP);  // closing switch grounds the pin, low = on = 1
  pinMode(serveButton, INPUT_PULLUP);  
  Serial.println("wake up");  // get Proc. serial event to start up

}

void loop() 
{
  if (Serial.available() > 0) //type any char (in Proc app/ser.mon.) to read/send values once
  {
    // .read the serial buffer to clear it
    // you don't care about the value of the incoming byte
    Serial.read();   // was  int inByte =; I don't need the ch value 

    // read the analog sensors:
    leftValue = analogRead(leftSensor);
    rightValue = analogRead(rightSensor);

    // read the digital sensors:
    reset = !digitalRead(resetButton);
    serve = !digitalRead(serveButton);

      // each set of four readings prints on a line by itself:
//    Serial.print(leftValue); // w/ DEC option, float prints 8 decimals, 2 like this
//    Serial.print(",");
//    Serial.print(rightValue);  // prints the AR as int/flt
//    Serial.print(",");
//    Serial.print(reset);  // boolean, 0/1
//    Serial.print(",");
//    Serial.println(serve); // final \n used by Proc to read a 'line' of data
    // can concat by making ints (not flts) into Str obj; bool vars print OK as is
    Serial.println(String(leftValue) + "," + String(rightValue) + "," + reset + "," + serve);
    // delay (1000);
  }  // end if
} // end loop
