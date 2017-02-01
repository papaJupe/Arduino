/* was ND's _03lois (who is lois?) used case struct to set flashing rate for position indicator; had
many problems, fixed here. Further improvements in v. 04
*/

//#define projectY 0xFF0099
//
//#define dis1 projectY
//#define dis2 0x400080
//#define dis3 0x300060
//#define dis4 0x200040
//#define dis5 0x100020

#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip?
#define NUM_LEDS 158    //for test strip
#define DATA_PIN 6
#define ENABLED_PIN A2
#define BALL_IN_PIN A0
#define POSITION_PIN A3  // analog position indicator

CRGB leds[NUM_LEDS], color;
unsigned long time = 0;
int normalizedPosition, cycleDuration;
int count =0;  // for sane print rate when debugging

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(9600);
  // I'm using switch to pull down digital input pins
  pinMode(ENABLED_PIN, INPUT_PULLUP);
  pinMode(BALL_IN_PIN, INPUT_PULLUP);
  pinMode(POSITION_PIN, INPUT);  // this is default anyway
  cycleDuration = 0;

  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = red;   // red indicates stopped status
  }
  FastLED.show();
  Serial.print("Initialized.\n");
  Serial.print("cycle duration = "), Serial.println(cycleDuration);

  delay(1000);

}

void loop()
{
  time = millis();
  count++; // needed only for debugging
  if (digitalRead(ENABLED_PIN) != 1)  // true when switch pulls down
  {
    enabled();
  } 
  else
  {
    disable();
  }

  FastLED.show();

  // print cycDur every 30 loops, 1.5 sec
  if(count % 30 == 0)
    Serial.print("cycle duration = "), Serial.println(cycleDuration);
  delay(50);
}  // end loop

///////////////////

void enabled()
{

  if ( digitalRead(BALL_IN_PIN) != 1) // if ball in ... make all green
  {  // tgt position: centered and non=centered cases 0 - 5
    normalizedPosition = map(analogRead(POSITION_PIN), 0, 1024, -100, 100); 
    if (abs(normalizedPosition) <= 10 )   // this was working OK in ND's
    {  // dead center, all lites solid green
      cycleDuration = 0;  // does nothing here, just rationalizes printout
      for (int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = green;  // all steady
      }
    } // end if dead ctr
    else 
    {  // not dead center  - ND's original switch/ case had many problems
      // durations too small, no breaks ... now works well

      switch ((abs(normalizedPosition) / 20) )   //  input -100 to +100, gives int 0-5
      {                                     // for these cases
      case 0:   // 10 - 20 
        cycleDuration = 120;  // must be lots slower than loop rate
        break;
      case 1:   // 20 - 30
        cycleDuration = 360;
        break;
      case 2: 
        cycleDuration = 600;
        break;
      case 3: 
        cycleDuration = 900;
        break;
      case 4: 
        cycleDuration = 1200;
        break;
      default:
        cycleDuration = 1500;
        break;
      } // end case list

      for (int i = 0; i < NUM_LEDS; i++) // flashing green indicates where target is 
          // and rate tells how close to center: farther = slower
      {
        leds[i] = (time % cycleDuration < cycleDuration / 2) ? green : black;  // flash all grn
      }
      if ( normalizedPosition < 0 )  // if  tgt left of ctr, color 1st half red (2nd half still flashing grn)
        for (int i = 0; i < NUM_LEDS / 2; i++) 
        {  
          leds[i] = red;   
        }
      else  // target normPosi must be rt of ctr, color 2nd half of string (1st half still flashing grn)
      for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {    
        leds[i] = red;  
      }
    }  // end else, not-centered actions

  } 
  else // bot is enabled but ball not in --> solid blue
  {
    for (int i = 0 ; i <= NUM_LEDS; i++)
    {
      leds[i] = blue;
    }
  }

}  // end enabled

void disable()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = yellow;
  }
}  // end disable













