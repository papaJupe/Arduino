/*
turret_and_interruptA  -- derived from miller's code, for control of turret with detector switch
   Revision A, putting debounce in Interrupts, probably faulty ISRs (local var no good?)
	CW switch should go to pin 2, CCW switch to pin 3
*/

#include <Servo.h>                  // Need this library to send servo signal for VexPro controller

Servo turret;                       // Create servo object to control motor controller of turret

int potpinA0 = A0;                  // Analog pin used to read the rate potentiometer
int potpinA1 = A1;                  // Analog pin used for location (positioning) potentiometer

                     // 3 global variables, will be used in various functions
int wantedPosition = 0;    // reads the value from the analog pin controlling position
volatile byte currentPosition = 0;     // Keep track of which gear cog turret is on; 'volatile' because reset by external data
              // initialization of these is optional; I'm setting them to neutral, stopped values

//====================================
void setup()
{             // Uno uses digital pin 2 (0), 3 (1) as interrupts 0 and 1; Mega has 4 more, D21 (2), 20 (3), 19 (4), 18 (5) 
  pinMode(2,INPUT);     // 2 = Interrupt #0; will go low when (+) switch closes; may be redundant:
  pinMode(3,INPUT);     // 3 = Interrupt #1; will go low when (-) switch closes; attachInterrupt may set Mode automatically
  digitalWrite(2, HIGH);   // enable internal pullup on 2,3; switch closing will pull pin LOW momentarily
  digitalWrite(3, HIGH);      // and reopening will pull again to HIGH, which we detect with RISING
  
  pinMode(9,OUTPUT);     // sends direction & rate 'servo mode' signal to motor controller
  
  turret.attach(9);            // Attaches the servo object to pin 9 for motor control
  
  attachInterrupt(0, up_1, RISING);  // D2 pin, reads switch that counts the cog number up;
                                   // RISING since it's the low-high transition upon switch reopening
                                  //// you want to do the interrupt
  
  attachInterrupt(1, down_1, RISING); // D3 pin, Reads switch that counts the cog number down
//  
//               // these LED's let you see the detection switch in action; same inputs wired to Interrupt pins
//  pinMode(4, INPUT);                 // Also read switch #1, the Interrupt on pin D2 
//  pinMode(10, OUTPUT);               // LED #1, green LED goes off when switch closes on CW click, otherwise on
//  pinMode(5, INPUT);                 // Also read switch #2
//  pinMode(11, OUTPUT);               // LED #2, red LED, goes off when switch closes on CCW click, otherwise on
//  digitalWrite(4, HIGH);   // enable internal pullup on 4,5 also; switch closing will pull it LOW momentarily
//  digitalWrite(5, HIGH);
}  / end setup
//-------------------------------------
void loop()
{                       // these var's are local to loop, but set only once
  static int rate = 0;                   // motor speed
  static int prevRate = 0;            //  so we print rate only if value changed
  static int prevVal = 0;             // previous wanted position, to print only if changed
  static int wasPosition = 0;        //previous actual position, to print only if changed
  rate = analogRead(potpinA0);   // read the value of the rate pot
   
  map(rate,0,1023,0,30);      // just to set speed; direction is set below
  
  if (rate != prevRate)   // only print if rate changes; this is scaled rate, so it will be more stable, print less often
    {Serial.print("rate is "); Serial.println(rate);}
     
  prevRate = rate;  // new value each loop, to see if it changes

  wantedPosition  = analogRead(potpinA1);    // reads the value of the position pot (between 0 and 1023)
  
  wantedPosition  = map(wantedPosition, 0, 1023, -10, 10);    // scale it to a position (stop at 10 clicks CCW or CW or somewhere between)
                // pot = zero volts tells position routine to go 10 clicks CCW, 5 v. tells it to go 10 clicks CW, 2.5 v, middle, would be stopped

  if (wantedPosition != prevVal)   // only print if changed; using scaled position val, so it prints less often
     {Serial.print("wantedPosition is "); Serial.println(wantedPosition);}

 prevVal = wantedPosition;   // reset each loop, to detect changes for printing

 int dir = wantedPosition - currentPosition;   // direction to turn, CW is +, CCW is -
              // e.g. starting at currentPosition 0, if wantedPosition is +10, you want dir to be +10; then, when currentPosition is 10, dir = 0, and motor stops;
              //when you get a new wantedPosition from pot, say -5, you want new dir to be -15, etc.
  
  if (dir > 0)   // i.e. if wantedPosition is > currentPosition, you're not at target, you want CW rotation
  {  interrupts();      // enable interrupts to track currentPosition
    turret.write(90+rate);   // @ 90 deg = motor stopped, so you tell it to turn CW at 'rate'
  }
  else if (dir < 0)  // wantedPosition is < current currentPosition, you're not at target wantedPosition,
                           // so you want CCW rotation
  {        
    interrupts();           // you want interrupt to reset currentPosition
    turret.write(90-rate);    // thus if 'rate' = 10, you get slow CCW rotation
  }
  else         // it must be zero (wantedPosition = currentPosition), so you write neutral, stopped value to motor
  {  
    turret.write(90);        // motor is off at 90
    noInterrupts();         // ignore interrupts if at target position
  }
  
  if(currentPosition != wasPosition)
    {Serial.print("currentPosition is "); Serial.println(currentPosition);
    wasPosition = currentPosition;
    }
  
  delay(50);     
 
}  // end loop

//------------- functions called by the + and - interrupts to update your (currentPosition)

void up_1()     // you've got a CW (+) click interrupt, so augment currentPosition (global);

{      // only works if this local variable LIT holds value between interrupts being called ??? dubious
  static unsigned long last_interrupt_time = 0;  // static sets it once, then not changed by repeated function calls ???
  unsigned long interrupt_time = millis();       //  millis() is not updated inside ISR but OK to use its value at instant of interrupt
 
     // If interrupts come faster than 200ms, assume it's a bounce and ignore; this is a long time for bounce
  if (interrupt_time - last_interrupt_time > 200)
    {
     ++currentPosition;       // augment currentPosition
     last_interrupt_time = interrupt_time;  // should be reset only on good signal, > 200 ms from a valid one
    }
}
//--------------------------------------
void down_1()     // you got a CCW (-) click interrupt, so you decrement currentPosition
{
  static unsigned long last_interrupt_time = 0;  // static is set once, not changed by repeated function calls
  unsigned long interrupt_time = millis();       // millis() is not updated inside ISR but OK to use its value at instant of interrupt
 
     // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
    {
      --currentPosition;        // decrement currentPosition          
      last_interrupt_time = interrupt_time;  // should be reset only on good signal, > 200 ms from a valid one
    }
}
