/*
ReadPoToPWM_A --  reads an analog input on pin Ax, converts it to voltage, 
		and prints the result to the serial monitor if changed; sends scaled PWM 
		to digital pin. Attach the center pin of a potentiometer to pin A4, and 
                the outside pins to +3.3 or 5V and ground.
		Uses: constant var, static, PWM output using analogWrite
 */
 
const int pwmPin=6;   // connect to LED or other thing you want to control
 // int wasSensor = 0;    //  var to track if input has changed

// the setup code runs once at power up, starting ser. mon. and when you press reset:
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);  // for printout of values
  //  pinMode(6, OUTPUT);  don't need if using analOut as below
  //  pinMode(7, OUTPUT);
  //  digitalWrite(7, HIGH);  // ~ 5v source, used to compare to full output voltage
}

//  loop runs code over and over:
void loop() 
{
  static int wasSensor = 0;  // init loc var to 0, just once; then I only print v. if it changes
  // read the input on analog pin:
  int sensorValue = analogRead(A3);
  
  // Convert the analog reading (from 0 - 675 for 3.3v, -1023 for 5.0) to a voltage:
  float potvolt = sensorValue * (5.0 / 1023.0);  // actual voltage pot is sending, not used now
  
  int pwmValue=map(sensorValue, 0, 1023, 0, 125);  // define new loc variable 
            //  and map sensor input range to analogWrite range of 0-255
  
    // write the pwm intensity value to an LED or something
  analogWrite(pwmPin, pwmValue);  // sends a value 0-255 to that digital (PWM capable) pin
  float outVolt = (5.0 * pwmValue / 255);  // actual analog voltage output as PWM
  
  if (sensorValue != wasSensor)  // only print if changed 
    {
    Serial.print("PWM signal is: ");
    Serial.println(pwmValue);
    Serial.print("output voltage is: ");
    Serial.println(outVolt);
    Serial.println();
    wasSensor=sensorValue;      // store for next loop
    
     }  //  end if
  delay(100);
}
