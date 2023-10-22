// V1 was maker orig, much modified by AM in V3, 2017
// C deploy to nano may need 328b (old bootloader); post diode burnout and fix,
// testing 230411, sl. increase duty_eMagnet --> 36, try different mode code--
// no big change, kept mostly same

#include <PWM.h>
// >>>>>>> origin/master
// PWM lib at https://code.google.com/archive/p/arduino-pwm-frequency-library/downloads
#include <elapsedMillis.h>  // make timing event less damaging to run speed

// #define DEBUG
//uncomment to view vars in serial monitor (will see board LED pulsing)
//mode Switch may need to be pressed longer in debug mode to change mode

// Base frequency and trimmer Ranges -- PWM lib customize timer freq.
#define BASE_FREQ 70 // 80 is on the spot for many flowers, can try +/-5Hz   
//Set to 79.8 so that light banding is not too bad if video at 1/80 sec
#define MIN_FREQ_OFFSET 0.2
#define MAX_FREQ_OFFSET 5.0  // 

// value of max/min_brite, adjusted by lite pot, feeds duty_led var which
//  ranges from 10-->2; i think these set pulse width as % of 255 so
//  actually very short flash of lite string; 1.25ms, ~10% of each cycle time
#define MIN_BRIGHT 2.0     // light off shows full oscillating effect
#define MAX_BRIGHT 12.0  // flickering will occur if too high; on too long?

elapsedMillis timeElapsed; // used in debug printout, could be local var

const byte LedStriPin = 10;  // PWM pin for LED strip control
const byte eMagnetPin = 3;     // pin for ElectroMagnet control
const byte BordLedPin = 13;      // pin for on-board LED
const byte modeButtonPin = 6;       // pin for mode selection, momentary sw1

boolean bordLedON = false;
boolean strobeON = false;
boolean mode_changed = true;

// delays for debugging mode
unsigned int interval = 400;  // ms between blinks of the on-board LED
unsigned int prInterval = 5000; // interval to Ser.print in debug mode


byte mode = 1; // can toggle mode w/ momentary sw1
//mode 1 = base slow motion (when powering on) = ~70 hz
//mode 2 = base freq + 7
//mode 3 = base freq + 14
//mode 4 = magnet off
//mode 5 = completely off

byte buttonState = 0;        // current state of the button
byte lastButtonState = 0;    // previous state of the button

float freq_offset = 0.2;
float duty_eMagnet = 24;   // don't overheat magnet with duty cycle too high
// better to adjust force using magnet position
float freq_eMagnet = BASE_FREQ;
float duty_led = 7;
float freq_led = freq_eMagnet + freq_offset;

int lastBrightnessValue = 0;

//**************************************************
void setup()
{
  Serial.begin(9600);

  pinMode(BordLedPin, OUTPUT);      // on board LED (pulses in debug mode)
  pinMode(modeButtonPin, INPUT_PULLUP); // Mode button, high until pressed, then low (ON)

  //initialize all timers except for 0, time keeping functions
  InitTimersSafe();

  //sets the frequency for the specified pin (fx from PWM lib)
  bool success = SetPinFrequencySafe(LedStriPin, freq_led);

  bool success2 = SetPinFrequencySafe(eMagnetPin, freq_eMagnet);

  // if the pin frequency was set successfully, turn Ardu board LED on
  if (success and success2)
  {
    digitalWrite(BordLedPin, HIGH);
    bordLedON = true;
  }
}  // end setup

//**************************************************************
void loop()
{ static uint32_t starTime = millis();

  if (mode_changed == true)
  {
    if (mode == 1)  //normal slow motion mode (power on)
    {
      freq_eMagnet = BASE_FREQ;
      SetPinFrequencySafe(eMagnetPin, freq_eMagnet);
      delay(5);
      eMagnet_on();
      strobeON = true;
    }
    //    else if (mode == 2)  // distorted reality mode
    //    {
    //      //frequency doubling done in main loop
    //    }

    else if (mode == 2)  // faster eMag
    {
      freq_eMagnet = BASE_FREQ + 7;
      SetPinFrequencySafe(eMagnetPin, freq_eMagnet);
      delay(5);
      //Serial.println(freq_eMagnet);
      eMagnet_on();
      strobeON = true;
    }
    else if (mode == 3)  // faster eMag
    {
      freq_eMagnet = BASE_FREQ + 14;
      SetPinFrequencySafe(eMagnetPin, freq_eMagnet);
      delay(5);
      //Serial.println(freq_eMagnet);
      eMagnet_on();
      strobeON = true;
    }

    else if (mode == 4)  // magnet off
    {
      eMagnet_off();
    }
    else if (mode == 5)  // all completely off
    {
      strobeON = false;
    }

    mode_changed = false;
  }

  //  Speed: 0.1 .. 5 Hz; ###L makes 1023 long but float should work
  // freq_offset = -(MAX_FREQ_OFFSET-MIN_FREQ_OFFSET)/1023L*analogRead(A1)+MAX_FREQ_OFFSET;
  // increasing pot R increases amt we shorten the offset from its MAX, so freq_offset
  // decreases, so freq_led decreases too, close to == freq_magnet
  freq_offset = MAX_FREQ_OFFSET - ((MAX_FREQ_OFFSET - MIN_FREQ_OFFSET) / 1023 * analogRead(A1));

  if (strobeON == true)
  { // Brightness: duty_led 2..10 ('MAX' (pulse width?) when pot is CCW full (0 v?)
    duty_led = MAX_BRIGHT - (((MAX_BRIGHT - MIN_BRIGHT) / 1023) * analogRead(A0));

    // freq_led = freq_eMagnet + (mode * freq_offset); // modes 2,3 multiply freq offset
    freq_led = freq_eMagnet + freq_offset;
    SetPinFrequencySafe(LedStriPin, freq_led);
    // transition to new briteness if lite pot changes duty_led value
    if (lastBrightnessValue < round(duty_led * 255 / 100)) // was dimmer - gradually brighten
    { // increasing by one each loop
      for (int i = lastBrightnessValue; i < round(duty_led * 255 / 100); i++)
      {
        pwmWrite(LedStriPin, i);
        delay(20);
      }
    }  // or when pot turned up, duty_led decreases, this condx loop causes dimming
    else if (lastBrightnessValue > round(duty_led * 255 / 100)) // was brighter - gradually dim
    {
      for (int i = lastBrightnessValue; i > round(duty_led * 255 / 100); i--)
      {
        pwmWrite(LedStriPin, i);
        delay(20);
      }
    }
    else  // no change in brightness (duty_led value), why do anything ??
      pwmWrite(LedStriPin, round(duty_led * 255 / 100));

    lastBrightnessValue = round(duty_led * 255 / 100);
  } // end else no change but strobe on
  
    else  //  mode = 5, strobe off
    {
      //gradually dim off
      for (int i = round(duty_led * 255 / 100); i > 0; i--)
      {
        pwmWrite(LedStriPin, i);
        delay(10);
      }
  
      duty_led = 0;
      pwmWrite(LedStriPin, 0);
      lastBrightnessValue = 0;
    }  // end unclear else

#ifdef DEBUG
  // pulse on-board LED -- lite and print redone so loop not delayed, was:
  //    digitalWrite(BdLedPin, HIGH); // LED on
  //    delay(300);
  //    digitalWrite(BdLedPin, LOW); // LED off
  //    delay(300);
  //    digitalWrite(BdLedPin, HIGH); // LED on
  //    delay(200);
  //    digitalWrite(BdLedPin, LOW); // LED off
  //    delay(1200);

  if (timeElapsed > interval) // for pulsing on-board LED ~ 2.5 hz
  {
    bordLedON = !bordLedON;       // toggle the state from HIGH to LOW etc.
    digitalWrite(BordLedPin, bordLedON);
    timeElapsed = 0;      // reset the timer to 0
  } // end on bd lite toggl

  if ( (millis() - starTime) > prInterval)
    // print current parameters every prInterval mseconds
  {
    Serial.print("\n  Frequency Offset: ");
    Serial.print(freq_offset);
    Serial.print("\n  Freq LED: ");
    Serial.print(freq_led);
    Serial.print("\n  Brightness (duty cyc): ");
    Serial.print(duty_led);
    Serial.print("\n  emagForce (duty cyc): ");
    Serial.print(duty_eMagnet);
    Serial.print("\n  Freq eMag: ");
    Serial.println(freq_eMagnet);

    starTime = millis();
  }  // end if it's time to print
#endif   // doing debug print, liteToggle

  // read the mode switch
  buttonState = digitalRead(modeButtonPin);

  // compare the buttonState to its previous state, might need more/less debounce
  if (buttonState != lastButtonState)
  {
    // if the state has changed, increment the counter
    if (buttonState == LOW)
    {
      mode++;
      if (mode > 5)
        mode = 1; // cycling value
      mode_changed = true ;
    }

    // delay a little for button debouncing
    delay(20);
  }  // end if butt state change

  lastButtonState = buttonState;

}  // end loop

//*********************************************************
void eMagnet_on()
{
  pwmWrite(eMagnetPin, round(duty_eMagnet * 255 / 100));
}

//*********************************************************
void eMagnet_off()
{
  pwmWrite(eMagnetPin, 0);
}
