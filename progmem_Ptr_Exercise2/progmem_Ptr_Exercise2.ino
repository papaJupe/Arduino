/*
 progmem_Ptr_Exercise2 -- example using progmem to:
 a. store lots of string constants, e.g. for printout, user feedback, debug, etc
 b. store array of strings (data) AND their names in progmem in standard way

 example here from Gammon page: http://gammon.com.au/progmem
 w/ mod and additions
Uses: free memory report, arrays into PROGMEM, strcpy_P, PSTR macro, PGM_P
*/

// #include <avr/pgmspace.h>  // expect needed for strcpy, already in build path ?

// variables created when compiling -- to report free memory
extern int __bss_end;
extern void * __brkval;

  // gammon here puts an array for each data string into progmem
const int NUMBER_OF_ELEMENTS = 10;
const char Message00 [] PROGMEM = "Twas bryllyg, and ye slythy toves"; 
const char Message01 [] PROGMEM = "Did gyre and gymble"; 
const char Message02 [] PROGMEM = "in ye wabe:"; 
const char Message03 [] PROGMEM = "All mimsy were ye borogoves; And ye mome raths outgrabe."; 
const char Message04 [] PROGMEM = "\"Beware the Jabberwock, my son! \n The jaws that bite, the claws that catch!"; 
const char Message05 [] PROGMEM = "Beware the Jubjub bird, and shun\n The frumious Bandersnatch!\""; 
const char Message06 [] PROGMEM = "He took his "; 
const char Message07 [] PROGMEM = "vorpal sword in hand:"; 
const char Message08 [] PROGMEM = "Long time the manxome foe he sought - "; 
const char Message09 [] PROGMEM = "So rested he by the Tumtum tree, \n And stood awhile in thought"; 
  // then he makes another array of the array names, which are ptr's, in prog space as well;
   // element type, const char *, is same as PGM_P
const char * const messages[NUMBER_OF_ELEMENTS] PROGMEM = 
   { 
   Message00, 
   Message01, 
   Message02, 
   Message03, 
   Message04, 
   Message05, 
   Message06, 
   Message07, 
   Message08, 
   Message09 
   };

void setup ()
  {
  Serial.begin (9600);  // prints some leftovers from out buffer; I don't know how to purge
  Serial.print (F("Free memory = "));
  Serial.println(memoryFree());

  for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
    {
    char * ptr = (char *) pgm_read_word (&messages [i]);
    char buffer [80]; // must be large enough!
    char buf [2];  // for i
    strcpy_P (buffer, PSTR( "element ")); // v. Gammon, macro reads str in pgm space into _P fx's
    itoa (i, buf, 10);  //  put loop index into buf as string
    strcat (buffer, buf); // add to buffer
    strcat (buffer, "\n");
    strcat_P (buffer, ptr);  // content at &messages[] is an array name (a ptr)
    Serial.println (buffer); // but strcpy_P et al can handle it, put its contents into buffer
    }   // end for loop

  }  // end of setup

void loop () { } 


//Output:
//
//Free memory = 1616
//Twas bryllyg, and ye slythy toves
//Did gyre and gymble
//in ye wabe:
//All mimsy were ye borogoves; And ye mome raths outgrabe.
//"Beware the Jabberwock, my son! 
// The jaws that bite, the claws that catch!
//Beware the Jubjub bird, and shun
// The frumious Bandersnatch!"
//He took his 
//vorpal sword in hand:
//Long time the manxome foe he sought - 
//So rested he by the Tumtum tree, 
// And stood awhile in thought.
//String memory used = 399  code for this I took out

// function to return size of free RAM
int memoryFree()
{
  int freeValue;

  if((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);

  return freeValue;
  
}  // end memFree



