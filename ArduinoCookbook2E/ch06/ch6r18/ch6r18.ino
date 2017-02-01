/*
  accel sketch
  simple sketch to output values on the x- and y-axes
  */

const int xPin = 0;  // analog input pins
const int yPin = 1;

void setup()
{
  Serial.begin(9600);  // note the higher than usual serial speed
}

void loop()
{
int xValue;  // values from accelerometer stored here
int yValue;

    xValue = analogRead(xPin);
    yValue = analogRead(yPin);

    Serial.print("X value = ");
    Serial.println(xValue);

    Serial.print("Y value = ");
    Serial.println(yValue);
    delay(100);
}
