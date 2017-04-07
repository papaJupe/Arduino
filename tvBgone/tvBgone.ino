/*
TB B Gone  
Last Updated: 22 Oct. 2016 By Gabriel Staples, http://www.ElectricRCAircraftGuy.com 
-My contact info is available by clicking the "Contact Me" tab at the top of my website.
 Mead mod 1610 cleaned up code, added explanations & questions, ~ interrupts and much
 else. v. B cleans up further by putting long fx in tabs, slimming codes to just NA, etc
 Uses: progmem data storage, special code lib, sleep lib, special timing fx
------------------------------------------------------------  
Semver (http://semver.org/) VERSION HISTORY (newest on top):  
(date format: yyyymmdd; ex: 20161022 is 22 Oct. 2016)  

TV-B-Gone for Arduino version 1.2, Oct 23 2010
Ported to Arduino by Ken Shirriff
http://www.righto.com/2010/11/improved-arduino-tv-b-gone.html (newer)

I added universality for EU (European Union) or NA (North America),
and Sleep mode to Ken's Arduino port
 -- Mitch Altman  18-Oct-2010
Thanks to ka1kjz for the code for adding Sleep
 <http://www.ka1kjz.com/561/adding-sleep-to-tv-b-gone-code/> <-- dead URL
some help w/ progmen here http://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html
------------------------------------------------------------
CIRCUIT:
------------------------------------------------------------
-NB: SEE "main.h" TO VERIFY DEFINED PINS
 Coded for Arduino328 (uno, nano); may need mod for others
 Connect an IR LED to pin D3 (IRLED) -- sends pulsed codes to TV; must be D3 (hard coded timers)
 Connect a visible LED to pin 13 (or use the built-in LED in many Arduinos) -- any OK.
 Connect a push-button between pin D2 (TRIGGER) and ground; must be this for interrupt to work
 -- push to send all codes, again to stop during sending
 Pin D5 (REGIONSWITCH) must be left HIGH for North America, or wire to ground for EU codes.

------------------------------------------------------------
LICENSE:
------------------------------------------------------------
Distributed under Creative Commons 2.5 -- Attribution & Share Alike

*/

#include <World_IR_Codes.h>   // this .h includes main.h -- in same folder
#include <avr/sleep.h>  // system lib in IDE, ? needs inclusion Y for ISR stuff

void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code );
void quickflashLEDx(uint8_t x );
void delay_ten_us(uint16_t us);
void quickflashLED(void );
uint8_t read_bits(uint8_t count);

#define putstring_nl(s) Serial.println(s)  // why alias these simple functions
#define putstring(s) Serial.print(s)
#define putnum_ud(n) Serial.print(n, DEC)
#define putnum_uh(n) Serial.print(n, HEX)
#define MAX_WAIT_TIME 65535 // tens of us (ie: 655.350ms)

  // not sure why extern -- ? because these values come from the lib
  // IrCode is a struct (in main.h) so these are arrays of pointers to structs
extern const IrCode* const NApowerCodes[] PROGMEM;
extern const IrCode* const EUpowerCodes[] PROGMEM;
extern uint8_t num_NAcodes, num_EUcodes;

/* from nongnu.org:
#define PGM_P   const char *
Used to declare a variable that is a pointer to a string (char array) in program space.
 */
uint8_t region;  // used in setup & sendAll
PGM_P code_ptr;

uint8_t bitsleft_r = 0;  // globals used in sendAll and read_bits
uint8_t bits_r=0;

    // pins defined in main.h, copy here for clarity
// #define LED LED_BUILTIN // LED indicator pin (usually 13)
// #define IRLED 3     // the IR sender LED ; must be PWM capable pin ?
// #define TRIGGER 2   // the button pin; NB: "hard-coded" in the sleepNow() function in the primary .ino file by means of using external interrupt 0, which is hard-wired to pin 2
// #define REGIONSWITCH 5  // pin to read region, high = NA

/*
 This project transmits a bunch of TV POWER on/off codes, one after another,
 pausing between, also pulses a visible LED once each time a POWER code is
 transmitted.  That is all TV-B-Gone does.  The tricky part of TV-B-Gone
 was collecting all the POWER codes, getting rid of the duplicates and
 near-duplicates (because if there is a duplicate, then one POWER code will
 turn the TV off, and the duplicate will turn it on again, which we certainly
 do not want).  I have compiled the most popular codes with the
 duplicates eliminated, both for North America (which is the same as Asia, as
 far as POWER codes are concerned -- even though much of Asia uses PAL video)
 and for Europe (which works for Australia, New Zealand, the Middle East, and
 other parts of the world that use PAL video).

*/


/*
 The C compiler normally will transfer all constants in prog memory into RAM when
 the program starts up.  Since this firmware has a table (powerCodes)
 that is too large to transfer into RAM, the C compiler needs to be told not to do that,
 keep it in program memory only. This is accomplished by the macro PROGMEM
 (used in the definition for powerCodes).  Since the C compiler expects
 that constants are in RAM, rather than in program memory, when accessing
 powerCodes, we need to use the pgm_read_word() and pgm_read_byte macros, and
 we need to use powerCodes as an address.  This is done with PGM_P (pointers) 
 defined below.
 
 For example,  to get a new powerCode, we first point to it with the
 following statement:
 PGM_P thecode_p = pgm_read_word(powerCodes+i);
 The next read from the powerCode is a byte that indicates the carrier
 frequency, read as follows:
 const uint8_t freq = pgm_read_byte(code_ptr++);
 After that is a byte that tells us how many 'onTime/offTime' pairs we have:
 const uint8_t numpairs = pgm_read_byte(code_ptr++);
 The next byte tells us the compression method. Since we are going to use a
 timing table to keep track of how to pulse the LED, and the tables are
 pretty short (usually only 4-8 entries), we can index into the table with only
 2 to 4 bits. Once we know the bit-packing-size we can decode the pairs
 const uint8_t bitcompression = pgm_read_byte(code_ptr++);
 Subsequent reads from the powerCode are n bits (same as the packing size)
 that index into another table in PROGMEM that actually stores the on/off times
 const PGM_P time_ptr = (PGM_P)pgm_read_word(code_ptr);
 */

void sendAllCodes() 
{
  
  uint16_t ontime, offtime;  // vars used in sendAllcodes only [and was param in xmitCode]
  uint8_t i,num_codes;  // just used in sendAllCodes

  
  bool endingEarly = false; // will be set to true if the user presses the button during code-sending 
      
  // determine region from REGIONSWITCH: 1 = NA, 0 = EU (defined in main.h)
  if (digitalRead(REGIONSWITCH)) {
    region = NA;
    num_codes = num_NAcodes;  // vals come from main.h ?
  }
  else {
    region = EU;
    num_codes = num_EUcodes;
  }

  // for every POWER code in our list
  for (i=0 ; i<num_codes; i++) 
  {
    PGM_P data_ptr;

    // print out the code # we are about to transmit
//    DEBUGP(putstring("\n\r\n\rCode #: ");
//    putnum_ud(i));

    // point to next POWER code, from the right database
    if (region == NA) {
      data_ptr = (PGM_P)pgm_read_word(NApowerCodes+i);
    }
    else {
      data_ptr = (PGM_P)pgm_read_word(EUpowerCodes+i);
    }

    // print out the address in ROM memory we're reading
//    DEBUGP(putstring("\n\rAddr: ");
//    putnum_uh((uint16_t)data_ptr));

    // Read the carrier frequency from the first byte of code structure
    const uint8_t freq = pgm_read_byte(data_ptr++);
    // set OCR for Timer1 to output this POWER code's carrier frequency
    OCR2A = freq;
    OCR2B = freq / 3; // 33% duty cycle

//    // Print out the frequency of the carrier and the PWM settings
//    DEBUGP(putstring("\n\rOCR1: ");
//    putnum_ud(freq);
//    );
//    DEBUGP(uint16_t x = (freq+1) * 2;
//    putstring("\n\rFreq: ");
//    putnum_ud(F_CPU/x);
//    );

    // Get the number of pairs, the second byte from the code struct
    const uint8_t numpairs = pgm_read_byte(data_ptr++);
    
//    DEBUGP(putstring("\n\rOn/off pairs: ");
//    putnum_ud(numpairs));

    // Get the number of bits we use to index into the timer table
    // This is the third byte of the structure
    const uint8_t bitcompression = pgm_read_byte(data_ptr++);
    
//    DEBUGP(putstring("\n\rCompression: ");
//    putnum_ud(bitcompression);
//    putstring("\n\r"));

    // Get pointer (address in memory) to pulse-times table
    // The address is 16-bits (2 byte, 1 word)
    PGM_P time_ptr = (PGM_P)pgm_read_word(data_ptr);
    data_ptr+=2;
    code_ptr = (PGM_P)pgm_read_word(data_ptr);


//DEVELOPMENTAL TESTING: 
//#if 0
//    // print out all of the pulse pairs
//    for (uint8_t k=0; k<numpairs; k++) {
//      uint8_t ti;
//      ti = (read_bits(bitcompression)) * 4;
//      // read the onTime and offTime from the program memory
//      ontime = pgm_read_word(time_ptr+ti);
//      offtime = pgm_read_word(time_ptr+ti+2);
//      DEBUGP(putstring("\n\rti = ");
//      putnum_ud(ti>>2);
//      putstring("\tPair = ");
//      putnum_ud(ontime));
//      DEBUGP(putstring("\t");
//      putnum_ud(offtime));
//    }
//    continue;
//#endif

    // Transmit all codeElements for this POWER code
    // (a codeElement is an onTime and an offTime)
    // transmitting onTime means pulsing the IR emitters at the carrier
    // frequency for the length of time specified in onTime
    // transmitting offTime means no output from the IR emitters for the
    // length of time specified in offTime
    
    // For EACH pair in this code....
    cli();  // don't interrupt code reading !
    // [Not appl. here: When accessing shared vars from the main program,] steps need to be taken to 
    // prevent wrong results if the interrupt is triggered in the middle of an operation.
    // The simplest and most common approach is to simply disable the global interrupt
    // setting with cli() and reenable with with sei(). v. infra
    for (uint8_t k=0; k<numpairs; k++) 
    {
      uint16_t ti;

      // Read the next 'n' bits as indicated by the compression variable
      // Then multiply by 4 because there are 2 timing numbers per pair
      // and each timing number is one word long, so 4 bytes total!
      ti = (read_bits(bitcompression)) * 4;

      // read the onTime and offTime from the program memory
      ontime = pgm_read_word(time_ptr+ti);  // read word 1 - ontime
      offtime = pgm_read_word(time_ptr+ti+2);  // read word 2 - offtime
      // transmit this codeElement (ontime and offtime)
      xmitCodeElement(ontime, offtime, (freq!=0));
    }
    sei();  // enables global interrupts v. www.pjrc.com/teensy/interrupts.html

    //Flush remaining bits, so that next code starts
    //with a fresh set of 8 bits.
    bitsleft_r=0;

    // visible indication that a code has been output.
    quickflashLED();
    
    // delay 120 milliseconds before transmitting next POWER code
    delay_ten_us(12000);

    // if user presses TRIGGER button, bringing pin low, stop transmission early 
    if (digitalRead(TRIGGER) == 0)  // interrupt is detached, so this reads the button
    {
      endingEarly = true;
      delay_ten_us(50000); // 500ms delay max unint16 = 65.4K
      quickflashLEDx(4);
      //pause for ~1.3 sec to give the user time to release the button so that the code sequence won't immediately start again.
      delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
      delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
      break; //exit the POWER code "for" loop
    } // end if
  } // end :  for every POWER code loop

  if (endingEarly==false)
  {
    //pause for ~1.3 sec, then flash the visible LED 8 times to indicate that we're done
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    quickflashLEDx(8);
  }
} //end of sendAllCodes

/* xmitCodeElement function is the 'workhorse' of transmitting IR codes.
 Given the on and off times, it turns on the PWM output on and off
 to generate one 'pair' from a long code. Each code has ~50 pairs! 
 */
  
 // This fx uses the AVR chip timers
void xmitCodeElement(uint16_t ontyme, uint16_t offtyme, uint8_t PWM_code )
{
  TCNT2 = 0;
  if(PWM_code) {
    // pinMode(IRLED, OUTPUT);  // why put in fx, already in setup ?
    // Fast PWM, setting top limit, divide by 8
    // Output to pin 3
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(WGM22) | _BV(CS21);
  }
  else 
  {
    // However some codes dont use PWM in which case we just turn the IR
    // LED on for the period of time.
    digitalWrite(IRLED, HIGH);
  }

  // Now we wait, allowing the PWM hardware to pulse out the carrier
  // frequency for the specified 'on' time
  delay_ten_us(ontyme);

  // Now we have to turn it off so disable the PWM output
  TCCR2A = 0;
  TCCR2B = 0;
  // And make sure that the IR LED is off too (since the PWM may have
  // been stopped while the LED is on!)
  digitalWrite(IRLED, LOW);

  // Now we wait for the specified 'off' time
  delay_ten_us(offtyme);
}  // end xmitCodeElement

/* read_bits is kind of a strange but very useful helper function.
 Because we are using compression, we index to the timer table
 not with a full 8-bit byte (which is wasteful) but 2 or 3 bits.
 Once code_ptr is set up to point to the right part of memory,
 this function will let us read 'count' bits at a time which
 it does by reading a byte into 'bits_r' and then buffering it.
 */

// we cant read more than 8 bits at a time so dont try!
uint8_t read_bits(uint8_t count)
{
  uint8_t i;
  uint8_t tmp=0;

  // we need to read back count bytes
  for (i=0; i<count; i++) 
  {
    // check if the 8-bit buffer we have has run out
    if (bitsleft_r == 0) {
      // in which case we read a new byte in
      bits_r = pgm_read_byte(code_ptr++);
      // and reset the buffer size (8 bites in a byte)
      bitsleft_r = 8;
    }
    // remove one bit
    bitsleft_r--;
    // and shift it off of the end of 'bits_r'
    tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count-1-i));
  }
  // return the selected bits in the LSB part of tmp
  return tmp;
}  // end read_bits()

 // ------------------------------------------------

void setup() 
{
  Serial.begin(9600);

  TCCR2A = 0;
  TCCR2B = 0;

  digitalWrite(LED, LOW);  // are these pins defined in header file ? why not in sketch
  digitalWrite(IRLED, LOW);
  pinMode(LED, OUTPUT);
  pinMode(IRLED, OUTPUT);
  pinMode(REGIONSWITCH, INPUT_PULLUP);
  pinMode(TRIGGER, INPUT_PULLUP);

  delay_ten_us(5000); // 50ms (5000x10 us) delay: let everything settle for a bit

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

//  // Debug output: indicate how big our database is
//  DEBUGP(putstring("\n\rNA Codesize: ");
//  putnum_ud(num_NAcodes);
//  );
//  DEBUGP(putstring("\n\rEU Codesize: ");
//  putnum_ud(num_EUcodes);
//  );

  // Tell the user what region we're in  --> 3 flashes is NA, 6 is EU
  if (region == NA)
    quickflashLEDx(3);
  else //region == EU
    quickflashLEDx(6);
}   //  end of setup 

 // ------------------------------------------------

void loop() 
{
  sleepNow(); // during this fx, press of trigger button wakes things up, detaches Interrupt then does more:
  
  // Super "ghetto" (but decent enough for this application) button debouncing:
  // if the user pushes the Trigger button, then wait a while to let the button stop bouncing, 
  // then start transmission of all POWER codes
  if (digitalRead(TRIGGER) == 0)  // i.e. if D2, trigger pin def in main.h, pressed, goes to 0
  {      // reading same event from inside sleepNow that woke us up?
    delay_ten_us(50000);  // delay 500ms, crude debounce
    sendAllCodes();  // if TRIGGER pressed during this fx, it quits, comes back to loop and sleep
  }
  // no loop delay coded; maybe enough in functions already?
}  // end loop


/****************************** LED AND DELAY FUNCTIONS ********/

// This function delays the specified number * 10 microseconds
// it is 'hardcoded' and is calibrated by adjusting DELAY_CNT
// in main.h. [Unless you are changing the crystal from 8MHz, dont  <-- old comment ?
// mess with this.]
// due to uint16_t datatype, max delay is 65535 tens of microseconds, or 655350 us = 655.350 ms. 
// NB: DELAY_CNT has been increased in main.h from 11 to 25 (last I checked) in order to allow 
// this function to work properly with 16MHz Arduinos (instead of 8MHz).

void delay_ten_us(uint16_t us) 
{
  uint8_t timer;
  while (us != 0) 
  {
    // for 8MHz we want to delay 80 cycles to = 10 microseconds
    // this code is tweaked to give about that amount. <-- old comment ?
    for (timer=0; timer <= DELAY_CNT; timer++) 
    {
      NOP;   // no operation, does nothing, just for timing
      NOP;
    }
    NOP;
    us--;
  }  // end while
} // end delay 10 us


// This function quickly pulses the visible LED
// This will indicate to the user that a code is being transmitted
void quickflashLED( void ) 
{
  digitalWrite(LED, HIGH);
  delay_ten_us(5000);   // 30 ms ON-time delay
  digitalWrite(LED, LOW);
}

// This function just flashes the visible LED a couple times, used to
// tell the user what region is selected
void quickflashLEDx( uint8_t x ) 
{
  quickflashLED();
  while(--x) 
  {
    delay_ten_us(25000); // 250 ms OFF-time delay between flashes, uint16 must be < 65.7K
    quickflashLED();
  }
}  // end quickFlashx



/*********** SLEEP and WAKE FUNCTIONS *******
 from kaqkjz: now dead link
 http://www.ka1kjz.com/561/adding-sleep-to-tv-b-gone-code/
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
