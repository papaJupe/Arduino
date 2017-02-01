/*
 * BlinkM sketch
 * This sketch continuously fades through the color wheel
 */

#include <Wire.h>

const int address = 0;  // Default I2C address for BlinkM

int color = 0; // a value from 0 to 255 representing the hue
byte R, G, B;  // the Red, Green, and Blue color components

void setup()
{
  Wire.begin(); // set up Arduino I2C support

  // turn on power pins for BlinkM
  pinMode(17, OUTPUT);    // pin 17 (analog out 3) provides +5V to BlinkM
  digitalWrite(17, HIGH);
  pinMode(16, OUTPUT);    // pin 16 (analog out 2) provides Ground
  digitalWrite(16, LOW);
}


void loop()
{
  int brightness = 255; // 255 is maximum brightness
  hueToRGB(color, brightness);  // call function to convert hue to RGB
  // write the RGB values to BlinkM

  Wire.beginTransmission(address);// join I2C, talk to BlinkM
  Wire.write('c');           // 'c' == fade to color
  Wire.write(R);             // value for red channel
  Wire.write(B);             // value for blue channel
  Wire.write(G);             // value for green channel
  Wire.endTransmission();    // leave I2C bus

  color++;           // increment the color
  if (color > 255)    
     color = 0;
     delay(10);
}
      

// function to convert a color to its Red, Green, and Blue components.
void hueToRGB( int hue, int brightness)
{
    unsigned int scaledHue = (hue * 6);
    unsigned int segment = scaledHue / 256; // segment 0 to 5 around color wheel
    unsigned int segmentOffset = scaledHue - (segment * 256); //segment position
    unsigned int complement = 0;
    unsigned int prev = (brightness * ( 255 -  segmentOffset)) / 256;
    unsigned int next = (brightness *  segmentOffset) / 256;
    switch (segment) {
        case 0:      // red
            R = brightness;
            G = next;
            B = complement;
        break;
        case 1:     // yellow
            R = prev;
            G = brightness;
            B = complement;
        break;
        case 2:     // green
            R = complement;
            G = brightness;
            B = next;
        break;
        case 3:    // cyan
            R = complement;
            G = prev;
            B = brightness;
        break;
        case 4:    // blue
            R = next;
            G = complement;
            B = brightness;
        break;
        case 5:      // magenta
        default:
            R = brightness;
            G = complement;
            B = prev;
        break;
    }
}