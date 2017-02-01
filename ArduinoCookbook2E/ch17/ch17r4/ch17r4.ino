/*
 * Write strings using Program memory (Flash)
 */
 
void setup()
{
  Serial.begin(9600); 
}

void loop()
{
  Serial.print(memoryFree());  // print the free memory
  Serial.print(' ');           // print a space

  Serial.print(F("arduino duemilanove "));  // print the string
  delay(1000);
}

// variables created by the build process when compiling the sketch
extern int __bss_end;
extern void *__brkval;

// function to return the amount of free RAM
int memoryFree()
{
  int freeValue;

  if ((int)__brkval == 0)
     freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);

  return freeValue;
}  
