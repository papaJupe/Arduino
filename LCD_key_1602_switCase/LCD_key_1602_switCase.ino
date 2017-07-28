/* LCD key 1602 -- get values from 5 keys on 1602 LCD with keypad and do things with them;
      no need for DFR library; need to tune for each board anal values
      Uses: LiquidCrystal lib,
*/

#include <LiquidCrystal.h>   // don't need the DFR lib with this code

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

// define some values used by the panel and buttons
int lcd_key = 5;      // int 0-5 sent to switch function, 5 is NONE
int anal_read  = 0;  // raw AR value on pin A0

#define btnRIGHT  0    // list of ints for switch fx and corresponding buttons
#define btnUP     1  // may redo for more rational use of keys
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons()
{   // function to read the buttons, returns a name that define makes an int
		// this could be done using an enum as well
    anal_read = analogRead(0);       // read the voltage (0-1024) the buttons send A0 

    // e.g. buttons when read are centered at these values: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are less than that
    // print out AR values for your board if these don't work

    if (anal_read > 1000) return btnNONE; 

    // For V1.1 use this threshold
    if (anal_read < 50)   return btnRIGHT;  
    if (anal_read < 250)  return btnUP; 
    if (anal_read < 450)  return btnDOWN; 
    if (anal_read < 650)  return btnLEFT; 
    if (anal_read < 850)  return btnSELECT;  

   // OR comment the above threshold and use custom values you measure, e.g.:
   /*
     if (anal_read < 50)   return btnRIGHT;  
     if (anal_read < 195)  return btnUP; 
     if (anal_read < 380)  return btnDOWN; 
     if (anal_read < 555)  return btnLEFT; 
     if (anal_read < 790)  return btnSELECT;   
   */

    return btnNONE;                // when all others fail, return this
    
}  // end read buttons

void setup()
{
   lcd.begin(16, 2);               // start the library with 16 col, 2 row
   lcd.setCursor(0,0);             // set the LCD cursor position top, left
   lcd.print("Try buttons");  // print a simple message on the LCD
}  // end setup
 
void loop()
{
   lcd.setCursor(9,1);             // move cursor to second line and 9 spaces over
   lcd.print(millis()/1000);       // display seconds elapsed since power-up

   lcd.setCursor(0,1);             // move to the begining of the second line
   lcd_key = read_LCD_buttons();   // read the buttons, put a number into variable for switch

   switch (lcd_key)
   {           // depending on which button was pushed, we perform an action;
               // looks like strings, but they are ints because of defines
       case btnRIGHT:
       {          //  if  "RIGHT"  pressed show this on the screen
            lcd.print("RIGHT ");
            break;
       }
       case btnLEFT:
       {
             lcd.print("LEFT "); //  if  "LEFT" show on the screen
             break;
       }    
       case btnUP:
       {
             lcd.print("UP ");  //   button "UP" 
             break;
       }
       case btnDOWN:
       {
             lcd.print("DOWN  ");  //  button "DOWN"
             break;
       }
       case btnSELECT:
       {
             lcd.print("SELECT");  //  if button "SELECT"
             break;
       }
       case btnNONE:
       {
             lcd.print("NONE  ");  //  No action will show "None" on the screen
             break;
       }
   }  // end switch
} // end loop
