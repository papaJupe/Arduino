/*
 * GLCDImage
 * Display an image defined in me.h
 */

#include <glcd.h>

#include "bitmaps/allBitmaps.h"       // all images in the bitmap folder

void setup()
{
  GLCD.Init();   // initialize the library
  GLCD.ClearScreen();
  GLCD.DrawBitmap(ArduinoIcon, 0,0); //draw the supplied bitmap
  delay(5000);
  GLCD.ClearScreen();
  GLCD.DrawBitmap(me, 0,0); //draw your bitmap
}

void  loop()
{

}
      
    
