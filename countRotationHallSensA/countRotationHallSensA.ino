/*
  count rotations with hall sensor
   display count on LCD display with buttons (read on A0, not used in program)
*/

#include <Streaming.h>  // lets lcd print float
#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

char msgs[5][16] = {"Right Key OK ",
                    "Up Key OK    ",
                    "Down Key OK  ",
                    "Left Key OK  ",
                    "Select Key OK"
                   };
//  read pin A0 then loop thru array to get right msg to print
int adc_key_val[5] = {50, 250, 450, 650, 850 }; // tune for each board
int NUM_KEYS = 5;
int adc_key_in;   // need loop code to read key or make fx() to do it
int key = -1;
int oldkey = -1;

volatile int rotaCount = 0;                // updated from ISR
volatile unsigned long last_interrupt_time = millis();

void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Serial.begin(9600);       // 9600 bps, for Ser. Mon. if on
  pinMode(2, INPUT_PULLUP);  // interrupt pin 0
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); // 5 v for sensor module
  pinMode(A0, INPUT_PULLUP); // sensing panel button press
  attachInterrupt(0, upCnt, FALLING);  // D2 pin attached to sensor
  // FALLING since magnet causes conduction, hi-lo transition @ D2
}

void loop()
{
  int keyV = analogRead(A0);
  if (keyV < 999)  // some button is pressed
  { // loop thru values in array to ID key
    for (int j = 0; j < NUM_KEYS; j++)
    { if (keyV < adc_key_val[j])
      {
        key = j;
        break;  // exit the for loop
      }  // end if
    } // end for, getting key ID

    // some action to take with key, or key press could toggle some var
    switch (key)
    {
      case 0:
        { //  if "RIGHT"  pressed show this on the screen
          // lcd.print("RIGHT ");
          break;
        }
      case 1:
        {
          // lcd.print("LEFT "); // if "LEFT" show on the screen
          break;
        }
      case 2:
        {
          // lcd.print("UP ");  // button "UP"
          break;
        }
      case 3:
        {
          // lcd.print("DOWN  ");  // button "DOWN"
          break;
        }
      case 4:
        { rotaCount = 0; // button press resets count
          //lcd.print("SELECT");  //  if button "SELECT"
          break;
        }
      default:
        {
          // lcd.print("NONE  ");  //  No action will show "None" on the screen
          break;
        }
    }  // end switch

  }  // end if some key pressed

  // else CM = 1; // no key pressed, default to ___

  delay(100);

  lcd.clear();
  lcd.print("count : ");
  lcd.print(rotaCount);

  //  if (CM) lcd.print(" CM");
  //  else lcd.print(" IN");
  // float format prints with 1 decimal digit
  //  if (CM) lcd << "count: " << _FLOAT(distAvg, 1) << " CM";
  //  else lcd << "dist: " << _FLOAT(distAvg, 1) << " IN";
  Serial.println(rotaCount);

}  // end loop


void upCnt()     // ISR increments count

{
  unsigned long interrupt_time = millis();  // millis() is not updated inside ISR but OK
  // to use its value at instant of interrupt

  // If any interrupt comes faster than ___ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 100)
  {
    ++rotaCount;       // incr count
    last_interrupt_time = interrupt_time;  // global var, should be reset only on good signal, > 100 ms from a valid one
  }
}  //end upCnt

