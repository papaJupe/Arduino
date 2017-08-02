#include <PWM.h>  
// PWM lib at https://code.google.com/archive/p/arduino-pwm-frequency-library/downloads
#include <elapsedMillis.h>

//#define DEBUG
//uncomment to view vars in serial monitor (will see board LED pulsing)
//Tactile Switch may need to be pressed longer in debug mode to change mode

//Base frequency and trimmer Ranges  
#define BASE_FREQ  79.8; // 80 is on the spot for many flowers, can play with this +/-5Hz   
//Set to 79.8 so that light banding is not too serious if videoing with shutter speed at 1/80 sec when light strobes at 80Hz
#define MIN_FREQ_OFFSET 0.1
#define MAX_FREQ_OFFSET 5.0
#define MIN_BRIGHT 2     // allows light to be off to reveal the full oscillating effect
#define MAX_BRIGHT 10.0  // too high and flickering will occur

elapsedMillis timeElapsed; // declare global if you don't want it reset every time loop runs

const byte LedStrPin = 10;     // pin for LED strip control (3 & 10 PWM capable)
const byte eMagnet = 3;        // pin for Electromagnet control
const byte BdLedPin = 13;      // pin for on-board LED
const byte ButtonSW = 6;       // pin for mode selection momentary sw1

boolean bdLedON = false;
boolean strobeON = false;
boolean mode_changed = true;
// delay in milliseconds between blinks of the ardu on-board LED and Ser.print vals
unsigned int interval = 400;
unsigned int prInterval = 5000;


byte mode = 1; //toggle mode w/ button, momentary SW1
//mode 1 = normal slow motion (power on)
//mode 2 = distorted reality mode
//mode 3 = magnet off
//mode 4 = completely off

byte buttonState = 0;        // current state of the button
byte lastButtonState = 0;    // previous state of the button

float freq_offset = 0.1;
float duty_eMagnet = 18;   // don't overheat magnet with duty cycle too high 
// Better to adjust force through magnet position
float freq_eMagnet = BASE_FREQ;  
float duty_led = 7;  
float freq_led = freq_eMagnet + freq_offset; 

int lastBrightnessValue = 0;

//**************************************************
void setup()
{
  Serial.begin(9600);

  pinMode(BdLedPin, OUTPUT);      // on board (pulse or not) LED
  pinMode(ButtonSW, INPUT_PULLUP); // Mode button, high until pressed, then low (ON)
    
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  //sets the frequency for the specified pin (fx from PWM lib)
  bool success = SetPinFrequencySafe(LedStrPin, freq_led);
  
  bool success2 = SetPinFrequencySafe(eMagnet, freq_eMagnet);
  
  // if the pin frequency was set successfully, turn Ardu board LED on
  if(success and success2) 
    digitalWrite(BdLedPin, HIGH); 
    bdLedON = true;   
}  // end setup

//**************************************************************
void loop()
{    static uint32_t starTime = millis();

  if (mode_changed == true)
  {
    if (mode == 1)  //normal slow motion mode (power on)
    {   
      freq_eMagnet = BASE_FREQ;
      eMagnet_on();    
      strobeON = true;
    }
    else if (mode == 2)  // distorted reality mode
    {
      //frequency doubling already done in main loop
    }
    else if (mode == 3)  // magnet off
    { 
      eMagnet_off();
    }
    else if (mode == 4)  // completely off
    {
      strobeON = false;
    }
    
    mode_changed = false; 
  }

        //  Speed: 0.1 .. 5 Hz    L makes it long but doubt needed, maybe float?
  // freq_offset = -(MAX_FREQ_OFFSET-MIN_FREQ_OFFSET)/1023L*analogRead(A1)+MAX_FREQ_OFFSET; 
  freq_offset = MAX_FREQ_OFFSET-(MAX_FREQ_OFFSET-MIN_FREQ_OFFSET)/1023*analogRead(A1);

  if (strobeON == true)
  {   // Brightness: duty_led 2..20  L makes it long but doubt needed
    duty_led = MAX_BRIGHT-(MAX_BRIGHT-MIN_BRIGHT)/1023*analogRead(A0);  
    freq_led = freq_eMagnet*mode+freq_offset;
    
    SetPinFrequencySafe(LedStrPin, freq_led);
    
    if (lastBrightnessValue < round(duty_led*255/100))  // previously dimmer - gradually brighten it
    {
      for (int i=lastBrightnessValue; i<round(duty_led*255/100); i++)
      {
        pwmWrite(LedStrPin, i);
        delay(30);
      }
    } 
    else if (lastBrightnessValue > round(duty_led*255/100)) // previously brighter - gradually dim it
    {
      for (int i=lastBrightnessValue; i>round(duty_led*255/100); i--)
      {
        pwmWrite(LedStrPin, i);
        delay(30);      
      }
    }
    else  //no change in brightness
      pwmWrite(LedStrPin, round(duty_led*255/100));   

    lastBrightnessValue = round(duty_led*255/100);
  }
  else
  {
    //gradually dim off
    for (int i=round(duty_led*255/100); i>0; i--)
    {
      pwmWrite(LedStrPin, i);
      delay(30);
    }
      
    duty_led = 0;      
    pwmWrite(LedStrPin, 0);
    lastBrightnessValue = 0;
  }

  #ifdef DEBUG
    // pulse on-board LED -- lite and print redone so loop not delayed
//    digitalWrite(BdLedPin, HIGH); // LED on
//    delay(300);
//    digitalWrite(BdLedPin, LOW); // LED off
//    delay(300); 
//    digitalWrite(BdLedPin, HIGH); // LED on
//    delay(200);
//    digitalWrite(BdLedPin, LOW); // LED off
//    delay(1200); 
    
     if (timeElapsed > interval) // for pulsing on board lite
  {				
      bdLedON = !bdLedON;         // toggle the state from HIGH to LOW etc. 
      digitalWrite(BdLedPin, bdLedON);
      timeElapsed = 0;      // reset the counter to 0
  } // end lite togg
  
    if ( (millis() - starTime) > prInterval)
    // print current parameters every prInt mseconds
    {
    Serial.print("\nFrequency Offset: "); 
    Serial.print(freq_offset);
    Serial.print("\n  Force: ");
    Serial.print(duty_eMagnet);
    Serial.print("\n  Freq Mag: ");
    Serial.print(freq_eMagnet);
    Serial.print("\n  Freq LED: ");
    Serial.print(freq_led);
    Serial.print("\n  Brightness: ");
    Serial.println(duty_led);
    
    starTime = millis();
    }  // end if time to print
  #endif   // to debug

  // read the mode switch
  buttonState = digitalRead(ButtonSW);

  // compare the buttonState to its previous state, might need more/less debounce
  if (buttonState != lastButtonState) 
  {
    // if the state has changed, increment the counter
    if (buttonState == LOW) 
        {    
          mode++;
          if (mode > 4)
            mode = 1; // rotating value
          mode_changed = true ;      
        }

    // delay a little for button debouncing
    delay(50);
  }  // end if butt state change

  lastButtonState = buttonState;
  
}  // end loop

//*********************************************************
void eMagnet_on() 
{
  pwmWrite(eMagnet, round(duty_eMagnet*255/100));
}

//*********************************************************
void eMagnet_off() 
{
  pwmWrite(eMagnet, 0);  
}
