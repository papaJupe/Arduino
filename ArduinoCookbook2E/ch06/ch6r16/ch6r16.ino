/*
  HM55bCompass sketch
  uses 'software SPI' serial protocol implemented using Arduino bit functions
  (see Chapter 3)
  prints compass angle to Serial Monitor
 */

const int enablePin = 2;
const int clockPin  = 3;
const int dataPin   = 4;

// command codes (from HM55B data sheet)
const byte COMMAND_LENGTH = 4;        // the number of bits in a command
const byte RESET_COMMAND = B0000;     // reset the chip
const byte MEASURE_COMMAND = B1000;   // start a measurement
const byte READ_DATA_COMMAND = B1100; // read data and end flag
const byte MEASUREMENT_READY = B1100; // value returned when measurement complete


int angle;

void setup()
{
  Serial.begin(9600);
  pinMode(enablePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
  reset();  // reset the compass module
}

void loop()
{
  startMeasurement();
  delay(40); // wait for the data to be ready
  if (readStatus()==MEASUREMENT_READY);  // check if the data is ready
  {
    angle = readMeasurement();          //read measurement and calculate angle
    Serial.print("Angle = ");
    Serial.println(angle); // print angle
  }
  delay(100);
}

void reset()
{
  pinMode(dataPin, OUTPUT);
  digitalWrite(enablePin, LOW);
  serialOut(RESET_COMMAND, COMMAND_LENGTH);
  digitalWrite(enablePin, HIGH);
}

void startMeasurement()
{
  pinMode(dataPin, OUTPUT);
  digitalWrite(enablePin, LOW);
  serialOut(MEASURE_COMMAND, COMMAND_LENGTH);
  digitalWrite(enablePin, HIGH);
}

int readStatus()
{
  int result = 0;
  pinMode(dataPin, OUTPUT);
  digitalWrite(enablePin, LOW);
  serialOut(READ_DATA_COMMAND, COMMAND_LENGTH);
  result = serialIn(4);
  return result;   // returns the status
}

int readMeasurement()
{
  int X_Data = 0;
  int Y_Data = 0;
  int calcAngle = 0;
  X_Data = serialIn(11); // Field strength in X
  Y_Data = serialIn(11); // and Y direction
  digitalWrite(enablePin, HIGH); // deselect chip
  calcAngle = atan2(-Y_Data , X_Data) / M_PI * 180; // angle is atan(-y/x)
  if(calcAngle < 0)
    calcAngle = calcAngle + 360; // angle from 0 to 259 instead of plus/minus 180
  return calcAngle;
}

void serialOut(int value, int numberOfBits)
{
  for(int i = numberOfBits; i > 0; i--) // shift the MSB first
  {
    digitalWrite(clockPin, LOW);
    if(bitRead(value, i-1) == 1)
      digitalWrite(dataPin, HIGH);
    else
      digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, HIGH);
  }
}

int serialIn(int numberOfBits)
{
  int result = 0;

  pinMode(dataPin, INPUT);
  for(int i = numberOfBits; i > 0; i--) // get the MSB first
  {
    digitalWrite(clockPin, HIGH);
    if (digitalRead(dataPin) == HIGH)
      result = (result << 1) + 1;
    else
      result = (result << 1) + 0;
    digitalWrite(clockPin, LOW);
  }

  // the following converts the result to a twos-complement negative number
  // if the most significant bit in the 11 bit data is 1
  if(bitRead(result, 11) == 1)
    result = (B11111000 << 8) | result; // twos complement negation

  return result;
}
      
    
