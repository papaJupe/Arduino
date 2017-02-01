

/*
 Last Updated: 22 Oct. 2016
 By Gabriel Staples, http://www.ElectricRCAircraftGuy.com 
 -My contact info is available by clicking the "Contact Me" tab at the top of my website.
 mead mod B cleans up messes: misuse of vars as (local) params; put long functions in separate tabs,
 still much debug code that could be cleared out or commented; __Nu.h reordered IRcode ptr list to 
 put my LG code closer to start, also purged all but few of EU codes. Now program 4K bytes smaller.
 
 Uses: hard coded timer, progmem for large array, pointer to progmem, data array in separate header,
 functions in separate tab, sleep, interrupt
 
 Note from GS:
 This project transmits a bunch of TV POWER on/off codes from a list,
 with a pause between each. To have a visible indication that it is
 transmitting, it also pulses a visible LED once each time a POWER code is
 sent. That is all TV-B-Gone does.  The tricky part of TV-B-Gone
 was collecting all of the POWER codes, and getting rid of the duplicates and
 near-duplicates because if there is a duplicate, then one POWER code will
 turn the TV off, and the duplicate will turn it on again.  I have compiled 
 the most popular codes with the duplicates eliminated, both for North America 
 (which is the same as Asia, as far as POWER codes are concerned -- even though 
 much of Asia uses PAL video) and for Europe (which works for Australia, New Zealand, 
 the Middle East, and other parts of the world that use PAL video).
 Note from AM:
 Testing 10/16 shows new TVs may not respond -- new codes needed or tech changed ? My
 gray IR led seemed to have much > power than clear ones -- output freq? I used only one
 and in parallel w/other (nl off) leds, all thru 1 100 ohm resistor, driven by 5 v from Ardu
 
 ------------------------------------------------------------  
 Semver (http://semver.org/) VERSION HISTORY (newest on top):  
 (date format: yyyymmdd; ex: 20161022 is 22 Oct. 2016)  
 
 TV-B-Gone for Arduino version 1.2, Oct 23 2010
 Ported to Arduino by Ken Shirriff
 http://www.righto.com/2010/11/improved-arduino-tv-b-gone.html (newer)
 
 I added universality for EU (European Union) or NA (North America),
 and Sleep mode to Ken's Arduino port -- Mitch Altman  18-Oct-2010
 Thanks to ka1kjz for the code for adding Sleep
 <http://www.ka1kjz.com/561/adding-sleep-to-tv-b-gone-code/> <-- dead URL 10/16
 some help w/ progmen here http://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html
 ------------------------------------------------------------
 CIRCUIT: SEE "main.h" TO VERIFY DEFINED PINS
 hard-Coded for Arduino328 (uno, nano); may need mod for others
 Connect an IR LED to pin D3 (IRLED) -- sends pulsed codes to TV; must be D3 (for timers to work)
 Connect a visible LED to pin 13 (or use the built-in LED) -- any OK.
 Connect a momentary switch pin D2 (TRIGGER) to ground; must be D2 for interrupt 0 to work
 -- push to send all codes, again to stop during sending, again to restart from top
 Pin D5 (REGIONSWITCH) must be left HIGH for North America, or wire to ground for EU codes.
 
 */

#include <World_IR_CodesNu.h>   // this .h 'includes' main.h -- in same folder
#include <avr/sleep.h>     // system lib already in IDE

// all vars used by this tab et al

uint8_t bitsleft_r;  // prob. must be global, set in sendAllC, used in read_bits
uint8_t bits_r = 0;   // global, tho only used in r_b, making local in read_bits failed
PGM_P code_ptr;   // used in read_bits and sendAll, prob needs to be global
/* from nongnu.org:  #define PGM_P   const char *
 Used to declare a variable that is a pointer to a string (ptr to char array) in program space.
 */
// not sure why extern; ? because these values come from the lib
extern uint8_t num_NAcodes, num_EUcodes;  // used in setup and sendAllC
// IrCode is a struct (def in main.h, data in other .h) so these are arrays of pointers to structs
extern const IrCode* const NApowerCodes[] PROGMEM;  // used in sendAllCodes only
extern const IrCode* const EUpowerCodes[] PROGMEM;
#define MAX_WAIT_TIME 65535 // tens of us (ie: 655.350ms) //used in sendAllCodes only
#define BUTTON_PRESSED 0   // used in loop only

uint16_t ontime, offtime; // vars used in sendAllcodes only (also were param in xmitCode b4 cleanup)
uint8_t  num_codes;  // just used in sendAllCodes
uint8_t region;  // just used in sendAllCodes

// all fx used in sketch -- need declar here or put in fx tabs if only used there?
void quickflashLEDx(uint8_t x );  // used in setup and sendAll
void quickflashLED(void );
void delay_ten_us(uint16_t us);  // used in setup, xmit, sendAll, quickFlash
// xmitCode called only by sendAllCodes()
void xmitCodeElement(uint16_t ontyme, uint16_t offtyme, uint8_t PWM_code );

uint8_t read_bits(uint8_t count); // called by sendAllCodes only, reads bits from code table

// used by debug only ?
#define putstring_nl(s) Serial.println(s)  // why alias these simple functions
#define putstring(s) Serial.print(s)
#define putnum_ud(n) Serial.print(n, DEC)
#define putnum_uh(n) Serial.print(n, HEX)
// could define LED and IRLED here ? rather than in main.h

/*  --- this describes the action in sendAllCodes function ---
 The C program normally will transfer all constants in prog memory into RAM 
 when the program starts up.  Since this firmware has a table (powerCodes)
 that is too large to transfer into RAM, the C compiler needs to be told to
 load it in program memory only. This is accomplished by the macro PROGMEM
 (used in the definition for powerCodes).  Since the C compiler expects
 that constants are in RAM rather than in program memory, when accessing
 powerCodes, we need to use the pgm_read_word() and pgm_read_byte macros, and
 we need to use powerCodes as an address.  This is done with PGM_P (pointers) 
 defined below.
 
 For example, when we start reading a new powerCode (struct), we first point 
 to it with the following statement:
 PGM_P data_ptr = pgm_read_word(XXpowerCodes+i); each val is ptr to a struct.
 The next read from the powerCode is a byte that indicates the carrier
 frequency:
 const uint8_t freq = pgm_read_byte(data_ptr++);
 After that a byte tells us how many 'onTime/offTime' pairs we have:
 const uint8_t numpairs = pgm_read_byte(data_ptr++);
 The next byte tells us the compression method. Since we are going to use a
 timing table to keep track of how to pulse the LED, and the tables are
 pretty short (usually only 4-8 entries), we can index into the table with only
 2 to 4 bits. Once we know the bit-packing-size we can decode the pairs:
 const uint8_t bitcompression = pgm_read_byte(data_ptr++);
 Subsequent reads from the powerCode are n bits (same as the packing size)
 that index into  2 other tables in PROGMEM that actually stores the coding key
 const PGM_P time_ptr = (PGM_P)pgm_read_word(data_ptr);
 and the on/off bits themselves:
 code_ptr = (PGM_P)pgm_read_word(data_ptr);
 
 SendAllCodes then uses 2 other functions readBits and xmitCode to decode
 the data further and transmit to the IRLED
 */

// -----------------------------------------

void setup()
{
  Serial.begin(9600); // used only in debug mode ?

  TCCR2A = 0;
  TCCR2B = 0;

  digitalWrite(LED, LOW);  // pins defined in main.header file; ? why not in sketch
  digitalWrite(IRLED, LOW);   // this must be D3 for hard-code timers to work
  pinMode(LED, OUTPUT);
  pinMode(IRLED, OUTPUT);
  pinMode(REGIONSWITCH, INPUT_PULLUP);  // high unless pulled down
  pinMode(TRIGGER, INPUT_PULLUP);

  delay_ten_us(5000); // 50ms (5000 x 10 us) delay: let everything settle

  // determine region
  if (digitalRead(REGIONSWITCH)) 
  {
    region = NA;   // region is typed as byte, NA gets value 1 (high) from main.h
    DEBUGP(putstring_nl("NA"));
  }
  else 
  {
    region = EU;
    DEBUGP(putstring_nl("EU"));
  }

  // Debug output: indicate how big our database is
  DEBUGP(putstring("\n\rNA Codesize: ");
  putnum_ud(num_NAcodes);
  );
  DEBUGP(putstring("\n\rEU Codesize: ");
  putnum_ud(num_EUcodes);
  );

  // Tell the user the region  --> 3 flashes is NA, 6 is EU
  if (region == NA)
    quickflashLEDx(3);
  else //region == EU
  quickflashLEDx(6);
}   //  end of setup ? Y

// -----------------------------------------

void loop() 
{
  sleepNow(); // during this fx, press of trigger button wakes things up then does more:

  // Super "ghetto" button debouncing (but decent enough for this application):
  // if the user pushes the Trigger button, then wait a while to let the button settle, 
  // then start sending all POWER codes
  // trigger is on iterrupt pin in sleepNow fx, but this is new dR not the usual volatile
  if (digitalRead(TRIGGER) == BUTTON_PRESSED)  // i.e. if D2, trigger pin def in main.h, goes to 0
  {
    delay_ten_us(50000);  // delay 500ms, crude debounce
    sendAllCodes();  // long fx, has its own tab
  }
  // no loop delay?; maybe enough delay in functions already?
}  // end loop


/************ Flash LED & DELAY FUNCTIONS *****************/

// This function delays the specified number * 10 microseconds;
// it is 'hardcoded' and is calibrated by adjusting DELAY_CNT
// in main.h. [Unless you are changing the crystal from 8MHz, dont  <-- old comment ?
// mess with this.]
// due to uint16_t datatype, max delay is 65535 tens of microseconds, or 655350 us = 655.350 ms. 
// NB: DELAY_CNT has been increased in main.h from 11 to 25 (last I checked) in order
// to work properly with 16MHz Arduinos.

void delay_ten_us(uint16_t us) 
{
  uint8_t timer;
  while (us != 0) 
  {
    // for 8MHz we want to delay 80 cycles to = 10 microseconds <--old comment?
    // this code is tweaked to give about that amount.
    for (timer=0; timer <= DELAY_CNT; timer++) 
    {
      NOP;   // no operation, does nothing, just for timing
      NOP;
    }
    NOP;
    us--;
  }  // end while
} // end delay 10 us


// This flashes the visible LED each time a code is sent
void quickflashLED( void ) 
{
  digitalWrite(LED, HIGH);
  delay_ten_us(3000);   // 30 ms ON-time delay
  digitalWrite(LED, LOW);
}

// This function just flashes the visible LED a few times @ startup, to
// tell the user what region is active, 3 for NA, 6 for EU
void quickflashLEDx( uint8_t x ) 
{
  quickflashLED();
  while(--x) 
  {
    delay_ten_us(42000);     // 420 ms OFF-time delay between flashes
    quickflashLED();
  }
}  // end quickFlashx



/******************** SLEEP and WAKE FUNCTIONS *****************
 * from kaqkjz: now dead link
 * http://www.ka1kjz.com/561/adding-sleep-to-tv-b-gone-code/
  there is some info about these functions deep in IDE: sleep.h file, none too clear to me
 */

void sleepNow()
{
  set_sleep_mode(TRIGGER);                    // sleep mode is set here

  sleep_enable();                             // enables the sleep bit in the mcucr register

  attachInterrupt(0, wakeUpNow, LOW);         // use interrupt 0 (pin 2) and run function
           // wakeUpNow when pin 2 gets LOW

  sleep_mode();           // if no interrupt, here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE ON WAKE -- not clear why

  sleep_disable();                            // first thing after waking, disable sleep

  detachInterrupt(0);                         // disables interrupt 0 as the wakeupnow code will
  // not be executed during normal runtime
}  // end sleepNow

void wakeUpNow()
{
  // any needed wakeup code can be placed here
}

