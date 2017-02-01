#include <avr/pgmspace.h> // for progmem

//create a string of 20 characters in progmem
const prog_uchar myText[] PROGMEM = "arduino duemilanove "; 

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print(memoryFree());  // print the free memory
  Serial.print(' ');           // print a space

  printP(myText);              // print the string
  delay(1000);
}

// function to print a PROGMEM string
void printP(const prog_uchar *str)
{
char c;

   while((c = pgm_read_byte(str++)))
     Serial.write(c);
}

// variables created by the build process when compiling the sketch
extern int __bss_end;
extern void *__brkval;

// function to return the amount of free RAM
int memoryFree(){
  int freeValue;

  if((int)__brkval == 0) freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else freeValue = ((int)&freeValue) - ((int)__brkval);
  return freeValue;
}