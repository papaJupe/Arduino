/*
turret_and_interrupt B, adapted for LCD display of want, now= positions
 	Revision A put debounced Interrupts, but used faulty ISR variables.
 	CW switch should go to pin 2, CCW switch to pin 3
 	Uses: Servo, Liq Crystal lib, interrupt, debounce, map, 8x2 display
*/

#include <Servo.h>                  // Need this library to send servo signal for VexPro controller
#include <LiquidCrystal.h>
      // initialize new display with the Arduino pins
LiquidCrystal lcd(12,11,4,5,6,7);  // display data to 8x2 LCD

Servo turret;                    // Create servo object to control motor controller of turret

int potpinA0 = A0;            // Analog pin used to read the rate potentiometer
int potpinA2 = A2;           // Analog pin used for positioning potentiometer
int dir;                            // will indicate direction turret needs to be turned
                                     // current position will be used in loop and interrupts
volatile int currentPosition = 0;     // Keeps track of which gear cog turret is on; 
                                     			 // 2 'volatiles' because reset by external events
volatile unsigned long last_interrupt_time = 0;  // global because both up and down interrupts 
                                              						  // will use same time
 
//====================================
void setup()
{             
       // Uno uses digital pin 2 (0), 3 (1) as interrupts 0 and 1; Mega has 4 more, D21 (2), 20 (3), 19 (4), 18 (5) 
  pinMode(2,INPUT_PULLUP);     // 2 = Interrupt #0; will go low when (+) switch closes; may be redundant:
  pinMode(3,INPUT_PULUP);     // 3 = Interrupt #1; will go low when (-) switch closes; attachInterrupt may set Mode automatically
                                // enable internal pullup on 2,3; switch closing will pull pin LOW momentarily
                                // and reopening will pull again to HIGH, which we detect with RISING
  
  pinMode(9,OUTPUT);     // sends direction & rate pulse width 'servo mode' signal to motor controller
  
  turret.attach(9);            // Attaches the servo object to pin 9 for motor control
  
  attachInterrupt(0, up_1, RISING);  // D2 pin, reads switch that counts the cog number up;
                                   // RISING since it's the low-high transition upon switch reopening
                                  // you want to do the interrupt, altho this could be bouncier ???
  
  attachInterrupt(1, down_1, RISING); // D3 pin, Reads switch that counts the cog number down
  
    // set up the LCD's number of columns and rows and clear screen 
  lcd.begin(8,2);
  lcd.clear();
  
               // two LED's let you see the detection switch in action; same inputs wired to Interrupt pins
 pinMode(4, INPUT_PULLUP);        // Also read switch #1, the Interrupt on pin D2 
 pinMode(10, OUTPUT);               // LED #1, green LED goes off when switch closes on CW click, otherwise on
 pinMode(5, INPUT_PULLUP);       // Also read switch #2, switch closing will pull it LOW momentarily
 pinMode(11, OUTPUT);               // LED #2, red LED, goes off when switch closes on CCW click, otherwise on

}  //end setup

//-------------------------------------
void loop()
{                       
       // these var's are local to loop, init once, then change by code
  static int rate = 0;                 // motor speed, set to fixed # for test
  static int prevRate = 0;            //  so we print rate only if value changed
  static int wantedPosition = 0;      // reads the value from the analog pin controlling position
  static int prevVal = 0;          // previous wanted position, to print only if changed
  static int wasPosition = 0;            //previous actual position, to print only if changed
  rate = analogRead(potpinA0);   // read the value of the rate pot
   
  rate = map(rate,0,1023,0,40);      // just to set speed; direction is set below
  
//  if (rate != prevRate)   // only print if rate changes; this is scaled rate, so it will be more stable, print less often
//    {Serial.print("rate is "); Serial.println(rate);}
     
// prevRate = rate;  // new value each loop, to see if it changes

  wantedPosition  = analogRead(potpinA2);          // reads the value of the position pot (between 0 and 1023)
  																// neutral around middle, 500
  wantedPosition  = map(wantedPosition, 0, 1000, -9, 9);    // scale it to a position (stop at 9 clicks CCW or CW or somewhere between)
                // pot = zero volts tells position routine to go CCW, 5 v. tells it to go CW, 2.5 v, middle, would be stopped

//  if (wantedPosition != prevVal)   // only print if changed; using scaled position val, so it prints less often
//     {Serial.print("wantedPosition is "); Serial.println(wantedPosition);}
//  prevVal = wantedPosition;   // reset each loop, to detect changes for printing

  lcd.clear();       // rewrite every loop
  lcd.print("want ");lcd.print(wantedPosition);

 dir = wantedPosition - currentPosition;   // direction to turn, CW is +, CCW is -
              // e.g. starting at currentPosition 0, if wantedPosition is +10, you want dir to be +10; then, when currentPosition is 10, dir = 0, and motor stops;
              //when you get a new wantedPosition from pot, say -5, you want new dir to be -15, etc.
  
  if (dir > 0)   // i.e. if wantedPosition is > currentPosition, you're not at target, you want CW rotation
  {
    turret.write(90+rate);   // @ 90 deg = motor stopped, so you tell it to turn CW at 'rate'
  }
  else if (dir < 0)  // wantedPosition is < current currentPosition, you're not at target wantedPosition
                           // so you want CCW rotation
  {        
    turret.write(90-rate);    // thus if 'rate' = 10, you get slow CCW rotation
  }
  else         // dir must be zero (wantedPosition = currentPosition), so you write neutral, stopped value to motor
  {  
    turret.write(90);        // motor is off at 90
  }
  
//  if(currentPosition != wasPosition)   // if it's changed
//    {
//    Serial.print("currentPosition is "); Serial.println(currentPosition);
//    wasPosition = currentPosition;
//    }

      lcd.setCursor(0,1);  // rewrite every loop; this is second line of display
      lcd.print("now= "); lcd.print(currentPosition);
      
     digitalWrite(10, digitalRead(4));   // CW LED reflects hi or lo (switch closed) state of input pin
     digitalWrite(11, digitalRead(5));  // CCW LED reflects state of that input pin
     												// expect them to be mostly on, flicker off when switch closes

  delay(100);     
 
}  // end loop

//------------- functions called by the + and - interrupts to update your (currentPosition)

void up_1()     // you've got a CW (+) click interrupt, so augment currentPosition value;
{   
  unsigned long interrupt_time = millis();     // millis() is not updated inside ISR but OK to use its value at instant of interrupt
 
     // If any interrupt comes faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) //  &&  dir != 0 would stop changes after target reached
    {
     ++currentPosition;       // augment currentPosition, if not at target position
     last_interrupt_time = interrupt_time;  // should be reset only on good signal, > 200 ms from a valid one
    }
}  //end up
//--------------------------------------
void down_1()     // you got a CCW (-) click interrupt, so you decrement currentPosition
{
  unsigned long interrupt_time = millis();   // millis() is not updated inside ISR but OK to use its value at instant of interrupt
 
     // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)  //  &&  dir != 0 would stop changes after target reached
    {
      --currentPosition;        // decrement currentPosition          
      last_interrupt_time = interrupt_time;  // should be reset only on good signal, > 200 ms from a valid one
    }
}  // end down

