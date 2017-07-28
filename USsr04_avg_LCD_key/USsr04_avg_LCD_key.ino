/* originally LCD 1602 Ultrasonic,
  From Sainsmart for their 1602 LCD display to read some US device;
  edited by AM, with some uncertainty about role of keypad switches;
  needed significant editing to work, edit 1707 for SR-04 US module;
  _avg vers. reads using exp moving average; _key version adds LCD
  module button actions; could be used for calibration etc
  Seems like when displaying inches, response very sluggish???
  might want to make float, print using Streaming.h (v. Sampl Synta)

   LCD RS pin to digital pin 8
   LCD Enable pin to digital pin 9
   LCD D4 pin to digital pin 4
   LCD D5 pin to digital pin 5
   LCD D6 pin to digital pin 6
   LCD D7 pin to digital pin 7
   LCD BL pin to digital pin 10
   KEY pin to analog pin 0
   Vcc pin to  +5         // above all hardwired w/ shield

   Trig pin to digital pin 11
   Echo pin to digital pin 13
  function :  when the serial port sends "a" to the Board, prints
  "holy sainsmart " on lcd --   no it won't unless you edit loop
*/

#include <LiquidCrystal.h>
#define TP 11     //Trigger pin
#define EP 13     //Echo pin

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // had 13 here, wrong, used for echo
const char numb[] = "0123456789";
char number_s[] = "000"; // these are used in LCD_conv_data fx, not sure how
char  rword;  // not clear why, how to use
char msgs[5][16] = {"Right Key OK ",
                    "Up Key OK    ",
                    "Down Key OK  ",
                    "Left Key OK  ",
                    "Select Key OK"
                   };
// could read pin A0 then loop thru these to get right msg to print
int adc_key_val[5] = {50, 250, 450, 650, 850 }; // tune for each board
int NUM_KEYS = 5;
int adc_key_in;   // need loop code to read key or make fx() to do it
int key = -1;
int oldkey = -1;
boolean CM = 1;      //Centimeter,  1 tells loop to display CM

void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  lcd.setCursor(0, 0);
  Serial.begin(9600);       // 9600 bps, for Ser. Mon. if on
  pinMode(TP, OUTPUT);      // TP output pin for trigger
  pinMode(EP, INPUT);       // EP input pin for echo
  pinMode(2, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  digitalWrite(2, HIGH); // 5 v for US module
}

void loop()
{ 
  static uint16_t distAvg = 69;
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
    // what action to take with key, or key press could toggle some var
       switch (key)
   {
       case 0:
       {    //  if "RIGHT"  pressed show this on the screen
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
       {     CM = 0; // sel button tells loop to display INches
             //lcd.print("SELECT");  //  if button "SELECT"
             break;
       }
       default:
       {     CM = 1;
             // lcd.print("NONE  ");  //  No action will show "None" on the screen
             break;
       }
   }  // end switch

  }  // end if some key pressed
  else CM =1;  // no key pressed, default to CM
  delay(100);
  long microseconds = TP_init(); // activates the pulser, gets uS back
  unsigned int dist = Distance(microseconds, CM);
  //  LCD_conv_data(distance_cm,number_s); unclear when/if needed
  // calc expon MA
  distAvg = round(((distAvg * 4) + dist) / 5);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("dist : ");
  lcd.print(distAvg);
  if (CM) lcd.print(" CM");
  else lcd.print(" IN");
  Serial.println(distAvg);
  // Serial.println(" Holy SainSmart");
}  // end loop, NB was no delay before

long TP_init()
{
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  digitalWrite(TP, HIGH); // make Trig pin HIGH for more than 10us
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  // pulse read waits for the echo pin to go HIGH, and
  // returns the length of the echo pulse in microseconds
  long microseconds = pulseIn(EP, HIGH);
  return microseconds;
}

uint16_t Distance(long time, int flag)  // returns unsigned int
{
  uint16_t distance;
  if (flag) // display in cm.
    distance = (time * 17) / 1000;
  //distance = time /29 / 2;  // was this, ? some other sensor type
  // distance  = ((Duration of high level)*(Sonic :340m/s))/2
  // = ((Duration of high level)*(Sonic :0.034 cm/us))/2
  // = ((Duration of high level)/(Sonic :29.4 cm/us))/2
  else  // 1122 ft/sec   13464 in/sec   0.0135 in/uS / 2 for bounce
    distance = round((time * 67) / 10000);  // INches, float might be better
  delay(20);
  return distance;
}


//convert number to string, ? need
void LCD_conv_data(unsigned int data_tem, char *number_s)
{
  char k = 0;
  if (data_tem != 0)
  {
    for (int i = 5; i != 0 && data_tem != 0; i--)
    {
      k = data_tem % 10;
      number_s[i] = numb[k];
      data_tem /= 10;
    }
  }
}
