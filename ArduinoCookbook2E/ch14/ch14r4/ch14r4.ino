/*
   XBeeAnalogReceive
  Read an analog value from an XBee API frame and set the brightness
  of an LED accordingly.
 */

#define LEDPIN 9

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
}

void loop() {

  if (Serial.available() >= 21) { // Wait until we have a mouthful of data

     if (Serial.read() == 0x7E) { // Start delimiter of a frame

      // Skip over the bytes in the API frame we don't care about
      for (int i = 0; i < 18; i++) {
        Serial.read();
      }

      // The next two bytes are the high and low bytes of the sensor reading
      int analogHigh = Serial.read();
      int analogLow = Serial.read();
      int analogValue = analogLow + (analogHigh * 256);

      // Scale the brightness to the Arduino PWM range
      int brightness = map(analogValue, 0, 1023, 0, 255);

      // Light the LED
      analogWrite(LEDPIN, brightness);
    }
  }
}
      
    
