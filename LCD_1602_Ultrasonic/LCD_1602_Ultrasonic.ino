/* LCD 1602 Ultrasonic,
From Sainsmart for their 1602 LCD display; and to read some US device; edited by AM, with some uncertainty about role of keypad switches, and use of D13; likely needs significant editing to work
  The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD BL pin to digital pin 10
 * KEY pin to analog pin 0
 * Vcc pin to  +5
 * Trig pin to digital pin 12
 * Echo pin to digital pin 13
function :  when the serial port send "a" to the Board, the Board will show "holy sainsmart " on lcd --   no it won't unless you edit loop
 */

#include <LiquidCrystal.h>
#define CM 1      //Centimeter
#define INC 0     //Inch
#define TP 12      //Trig_pin
#define EP 13      //Echo_pin

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);  // why 13 here? should be for echo
const char number_[]="0123456789";
char number_s[]="000";
char  rword;
char msgs[5][16] = {"Right Key OK ",
                    "Up Key OK    ",               
                    "Down Key OK  ",
                    "Left Key OK  ",
                    "Select Key OK" };

int adc_key_val[5] ={50, 200, 400, 600, 800 }; // tune for each board
int NUM_KEYS = 5;
int adc_key_in;   // are input keys being read somewhere?
int key=-1;
int oldkey=-1;

void setup()
{
  lcd.clear(); 
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  lcd.setCursor(0,0); 
  Serial.begin(9600);       // 9600 bps
  pinMode(TP,OUTPUT);       // set TP output for trigger  
  pinMode(EP,INPUT);        // set EP input for echo
}

void loop()
{

   long microseconds = TP_init();
   long distance_cm = Distance(microseconds, CM);
 //  LCD_cover_data(distance_cm,number_s);
   lcd.setCursor(0,0); 
   lcd.print(" Dis_CM :  "); 
   lcd.print(distance_cm); 
   lcd.print(" CM"); 
   Serial.println(distance_cm);
  // Serial.println(" Holy SainSmart");
}

long Distance(long time, int flag)
{
  long distance;
  if(flag)
    distance = time /29 / 2;  // Distance_CM  = ((Duration of high level)*(Sonic :340m/s))/2
       //     = ((Duration of high level)*(Sonic :0.034 cm/us))/2
                // = ((Duration of high level)/(Sonic :29.4 cm/us))/2
  else
    distance = time / 74 / 2;      // INC
  return distance;
}

long TP_init()
{                     
  digitalWrite(TP, LOW);                    
  delayMicroseconds(2);
  digitalWrite(TP, HIGH);  // put Trig pin to HIGH for more than 10us pulse 
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  long microseconds = pulseIn(EP,HIGH); // waits for the pin to go HIGH, and returns the length of the pulse in microseconds
  return microseconds;
}

       //cover number to string 
void LCD_cover_data(unsigned int data_tem, char *number_s)
{
  char k=0;
  if(data_tem!=0)
      {
     for(int i=5;i!=0&&data_tem!=0;i--)
        {
         k=data_tem%10;
         number_s[i]=number_[k];
         data_tem /= 10;
        }
      }
}