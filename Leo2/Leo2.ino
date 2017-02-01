JoyState_t joySt;
#include <SoftwareSerial.h>
#include <math.h>
int NBbuttons=14;
double k = 0;
void setup()
{
  for (int i=0;i<=NBbuttons;i++){
    pinMode(i+2,INPUT_PULLUP);
  }
  joySt.xAxis = 0;
  joySt.yAxis = 0;
  joySt.zAxis = 0;
  joySt.xRotAxis = 0;
  joySt.yRotAxis = 0;
  joySt.zRotAxis = 0;
  joySt.throttle = 0;
  joySt.rudder = 0;
  joySt.hatSw1 = 0;
  joySt.hatSw2 = 0;
  joySt.buttons = 0;
  
}

void loop()
{
  joySt.buttons=0;
  for (int i=0;i<=11;i++){
    if (digitalRead(i+2)==LOW){
      bitWrite(joySt.buttons,i,1);
    }
  }

  //joySt.throttle = analogRead(0);
  k = analogRead(0);
  k = .95*k- 380;
  joySt.yAxis = k;
  
  k = analogRead(1);
  k = .95*k- 380;
  joySt.xAxis = k;
  

  //commit changes to the joySt object
  Joystick.setState(&joySt);
  delay(10);
}

