           
/*
 * Brushed_H_Bridge_Direction sketch for Adafruit Motor shield
 * uses photo sensors to control motor direction
 * robot moves in the direction of a light 
 */

#include "AFMotor.h"  // adafruit motor shield library

AF_DCMotor leftMotor(3, MOTOR12_1KHZ); // motor #3, 1KHz pwm uses pin 5
AF_DCMotor rightMotor(4, MOTOR12_1KHZ); // motor #4, 1KHz pwm uses pin 6

const int MIN_PWM = 64;  // this can range from 0 to MAX_PWM;
const int MAX_PWM = 128; // this can range from around 50 to 255;

const int leftSensorPin = 0;  // analog pins with sensors
const int rightSensorPin = 1;
const int enableTurningPin = 16; // jumper to gnd to disable turning.

int sensorThreshold = 0;      // must have this much light on a sensor to move
int looks = 0;                // the number of attempts to turn and find light
int turnDelay = 500;          // adjust delay to allow robot to turn 90 degreeszz

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int leftVal  = analogRead(leftSensorPin);
  int rightVal = analogRead(rightSensorPin);
 
  if(sensorThreshold == 0){  // have the sensors been calibrated ?
     // if not, calibrate sensors to a little above the current average  
     sensorThreshold = ((leftVal + rightVal) / 2) + 100 ;  
  }   
 
  if( leftVal < sensorThreshold && rightVal < sensorThreshold)
  {
    int enableTurning = digitalRead(enableTurningPin);
    if(enableTurning && looks < 4)  // limit the number of consecutive looks
    {
      lookAround();
      looks = looks + 1;
      Serial.print("looks = "); Serial.println(looks);
    }
    else
    {
       setSpeed(leftMotor, 0 ); // stop motor until sufficient light
       setSpeed(rightMotor,0 ); 
    }
  }
  else
  {   
    // if there is adequate light to move ahead
    setSpeed(rightMotor, map(rightVal,0,1023, MIN_PWM, MAX_PWM));
    setSpeed(leftMotor,  map(leftVal ,0,1023, MIN_PWM, MAX_PWM));
    looks = 0; // reset the looks counter
  }
}

void lookAround()
{
  // rotate left 90 degrees
  Serial.println("looking");
  setSpeed(leftMotor, -MAX_PWM );
  setSpeed(rightMotor, MAX_PWM );
  delay(turnDelay);
  setSpeed(rightMotor, 0);
  setSpeed(leftMotor, 0 );
}

void setSpeed(AF_DCMotor &motor, int speed )
{
  if(speed < 0)
  {
    motor.run(BACKWARD);
    speed = -speed;
  }
  else
  {
    motor.run(FORWARD);
  }
  motor.setSpeed(speed);
}    
