#include <TimerOne.h>

const char SET_PERIOD_HEADER      = 'p';
const char SET_FREQUENCY_HEADER   = 'f';
const char SET_PULSE_WIDTH_HEADER = 'w';
const char SET_DUTY_CYCLE_HEADER  = 'c';


#define pwmRegister OCR1A    // the logical pin, can be set to OCR1B
const int   outPin =  9;     // the physical pin

long period = 1000;     // the period in microseconds
int duty = 512;         // duty as a range from 0 to 1024, 512 is 50% duty cycle

int prescale[] = {0,1,8,64,256,1024}; // the range of prescale values

void setup()
{
  Serial.begin(9600);
  pinMode(outPin, OUTPUT);
  Timer1.initialize(period);        // initialize timer1, 1000 microseconds
  Timer1.pwm(9, duty);              // setup pwm on pin 9, 50% duty cycle
}


void loop()
{
   processSerial();
}

void processSerial()
{
 static long val = 0;

  if ( Serial.available())
  {
    char ch = Serial.read();

    if(ch >= '0' && ch <= '9')              // is ch a number?
    {
       val = val * 10 + ch - '0';           // yes, accumulate the value
    }
    else if(ch == SET_PERIOD_HEADER)
    {
      period = val;
      Serial.print("Setting period to ");
      Serial.println(period);
      Timer1.setPeriod(period);
      Timer1.setPwmDuty(outPin, duty);  // don't change the duty cycle
      show();
      val = 0;
    }
    else if(ch == SET_FREQUENCY_HEADER)
    {
      if(val > 0)
      {
        Serial.print("Setting frequency to ");
        Serial.println(val);
        period = 1000000 / val;
        Timer1.setPeriod(period);
        Timer1.setPwmDuty(outPin, duty);  // don't change the duty cycle
      }
      show();
      val = 0;
    }
    else if(ch ==  SET_PULSE_WIDTH_HEADER)
    {
      if( setPulseWidth(val) ) {
         Serial.print("Setting Pulse width to ");
         Serial.println(val);
      }
      else
         Serial.println("Pulse width too long for current period");
      show();
      val = 0;
    }
    else if(ch == SET_DUTY_CYCLE_HEADER)
    {
     if( val >0 && val < 100)
     {
        Serial.print("Setting Duty Cycle to ");
        Serial.println(val);
        duty = map(val,1,99, 1, ICR1);
        pwmRegister = duty;
        show();
     }
     val = 0;
    }
  }
}

bool setPulseWidth(long microseconds)
{
  bool ret = false;

  int prescaleValue = prescale[Timer1.clockSelectBits];
  // calculate time per tick in ns
  long  precision = (F_CPU / 128000)  * prescaleValue  ;   
  period = precision * ICR1 / 1000; // period in microseconds
  if( microseconds < period)
  {
    duty = map(microseconds, 0,period, 0,1024);
    if( duty < 1)
      duty = 1;
    if(microseconds > 0 && duty < RESOLUTION)
    {
       Timer1.pwm(outPin, duty);
       ret = true;
    }
  }
  return ret;
}



void show()
{
   Serial.print("The period is ");
   Serial.println(period);
   Serial.print("Duty cycle is ");
   // pwmRegister is ICR1A or ICR1B
   Serial.print( map( pwmRegister, 0,ICR1, 1,99));      
   Serial.println("%");
   Serial.println();
}
      
    
