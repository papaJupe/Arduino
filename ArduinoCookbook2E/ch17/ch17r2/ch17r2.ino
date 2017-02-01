/*
F()functionExercise -- if it's useful in making array in progmem

Uses: F() function, FlashStringHelper, free memory report
*/


// #include <avr/pgmspace.h>  not needed

// filling ptr array here fails 
 // const __FlashStringHelper * myStrings[] = {F("abcdefg"), F("xbcdefg"), F("syzygy")};
// this works to declare the array of pointers, then fill it in setup
const __FlashStringHelper * myStrings[3];

// variables created by the build process when compiling the sketch
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

void setup()

{
  Serial.begin(9600);

  myStrings[0] = F("abcdefg");
  myStrings[1] = F("xwvutsrq");
  myStrings[2] = F("syzygy");
  Serial.println(myStrings[0]);
  Serial.println(myStrings[1]);
  Serial.print("free memo ");
  Serial.println(memoryFree());  // print the free memory
  Serial.println();
}

void loop()
{
}


