/* 
 Sensor Reader 3 from Igoe MTT, ch3, proj5, Context: Arduino
  orig SensorReader read two analog inputs and two digital inputs and prints 
  their values to ser. port (or mon.); I added if block for handshaking w/ Proc.; 
  this mod for testing, sends 3 ints from anal pin input
 
 Connections:  
 analog sensor(s) on analog input pin(s) (or pot simulates)
 */

const int leftSensor = 3;    // analog input for the left arm
const int rightSensor = 4;   // analog input for the right arm
const int thirdSensor = 5; 

int leftValue = 0;           // reading from the left 
int rightValue = 0;          // reading from the right
int thirdValue = 0;          // reading from the end pin

void setup() 
{
  // configure the serial connection:
  Serial.begin(9600);  // needs to be same as Proc's for it to read serial port
  // while (Serial.available() <= 0)  // setup halts here until something typed
  Serial.println("wake up");  // should get Proc. serial event to start up
}

void loop() 
{
  //  if (Serial.available() > 0)     // type char \r to read values x 1
  //  {
  //    // read the serial buffer to clear it
  //    // you don't care about the value of the incoming byte:
  //    Serial.read();   // was  int inByte = -- don't need the ch value, 
  //   }                 // just clears buffer

  // read the analog pins:
  leftValue = analogRead(leftSensor);
  rightValue = analogRead(rightSensor);
  thirdValue = analogRead(thirdSensor);
  //
  //    // print sends as char's, so OK in ser mon or Proc string input
  Serial.println(String(leftValue) + "," + String(rightValue) + "," + String(thirdValue));
  // Ser.write sends # as byte (0-255) so wraps around if higher than that
   //Serial.write(pinValue);
   
  delay (2000);

} // end loop
