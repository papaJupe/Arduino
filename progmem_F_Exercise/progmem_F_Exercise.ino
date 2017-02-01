/*
 progmem_F_Exercise -- test various uses:
 a. store lots of string constants, e.g. for printout, user feedback, debug, etc
 b. store array of strings (data) in progmem with simpler format
 
and what is use of PSTR(string_literal) vs F()?
Serial.print(PSTR("Hello, World")); which returns a char* does not work, would
 it work in setup? The problem is that de-referencing a char* returns the char stored in the Data space
(RAM) and not from the Code PROGMEM space (flash).  The F() macro changes the type from char*
to __FlashStringHelper*.

Uses: F() function, FlashStringHelper, free memory report
*/


// #include <avr/pgmspace.h>  not needed for F(), PGM_P, already in build path ?

// filling F() ptr array this way fails 
// const __FlashStringHelper * myStrings[] = {F("abcdefg"), F("xbcdefg"), F("syzygy")};
// this works to declare the array of F() pointers, then fill it in setup
const __FlashStringHelper * myStrings[3];

// variables created when compiling -- to report free memory
extern int __bss_end;
extern void * __brkval;

// function to return the amount of free RAM
int memoryFree()
{
  int freeValue;

  if((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);

  return freeValue;
}  // end memFree

// .print knows how to use F ptrs, but can other functions ?
// example said to work int val = strcmp_P(strval,(PGM_P) F("some string to test"));
// w/ cast back to ptr since PGM_P def as  const char * which avr_pgmspace.h fx can use
// full list of functions http://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html
// If I want to write a function that supports the F macro, what would the parameters be? Thanks
//Code:
//void MyFmacroFunction( const __FlashStringHelper * fsh );


void setup()
{
  Serial.begin(9600);
    // string used for feedback/debug
   Serial.println(F("something you should know"));
   // just put the F ptrs in the array elements, find by index, no labels
  myStrings[0] = F("abcdefg");
  myStrings[1] = F("xwvutsrq");
  myStrings[2] = F("syzygy");
  Serial.println(myStrings[0]);
  Serial.println(myStrings[2]);
  Serial.print("free memo ");
  Serial.println(memoryFree());  // print the free memory
  Serial.println();
}

void loop()
{
}
