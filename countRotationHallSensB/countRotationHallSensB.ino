/*
   count rotations with hall sensor
   display count on LCD display (optional)
*/

// #include <Streaming.h>  // lets lcd print float, not needed for ints
#include <LiquidCrystal.h>   

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

 // both vars updated from ISR
 // so must be declared volatile
volatile int rotaCount = 0;                      
volatile unsigned long last_interrupt_time = millis(); // used if debouncing

void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Serial.begin(9600);     // 9600 bps, for Ser. Mon. if on
  pinMode(2, INPUT_PULLUP);  // interrupt pin 0
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); // 5 v for sensor module
  attachInterrupt(0, upCnt, FALLING);  // D2 pin attached to sensor
  // FALLING since magnet causes conduction, hi-lo transition @ D2
}  //  end setup

void loop()
{

  lcd.clear();
  lcd.print("count : ");
  lcd.print(rotaCount);

  Serial.println(rotaCount);
  delay(100);
  
}  // end loop


void upCnt()     // ISR increments count

{ // debouncing not needed for hall sensor, ? needed for reed switch
  
  // unsigned long interrupt_time = millis();
  // millis() is not updated inside ISR but OK
  // to use its value at instant of interrupt

  // If any interrupt comes faster than ___ms, assume it's a bounce and ignore

  //  if (interrupt_time - last_interrupt_time > 50)
  //  {
  ++rotaCount;       // incr count
  // global var, should be reset only on good signal, > 50 ms from last good one
  // last_interrupt_time = interrupt_time; 
  //  }
}  //end upCnt

