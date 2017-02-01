/*
 * WaveShieldPlaySelection sketch
 *
 * play a selected WAV file
 *
 * Position of variable resistor slider when button pressed selects file to play
 *
 */

#include <FatReader.h>
#include <SdReader.h>

#include "WaveHC.h"
#include "WaveUtil.h"

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represents the WAV file
WaveHC wave;      // Only wave (audio) object - only one file played at a time


const int buttonPin = 15;
const int potPin = 0; // analog input pin 0

char * wavFiles[] = {
"1.WAV","2.WAV","3.WAV","4.WAV","5.WAV","6.WAV","7.WAV","8.WAV","9.WAV"};

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH); // turn on pull-up resistor

  if (!card.init())
  {
    // Something went wrong, sdErrorCheck prints an error number
    putstring_nl("Card init. failed!"); 
    sdErrorCheck();
    while(1);                           // then 'halt' - do nothing!
  }


  // enable optimized read - some cards may time out
  card.partialBlockRead(true);

  // find a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++)      // we have up to 5 slots to look in
  {
    if (vol.init(card, part))
      break;                            // found one so break out of this for loop
  }
  if (part == 5)                        // valid parts are 0 to 4, more not valid
  {
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();                     // Something went wrong, print the error
    while(1);                           // then 'halt' - do nothing!
  }

  // tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?

  // Try to open the root directory
  if (!root.openRoot(vol))
  {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }

  // if here then all the file prep succeeded.
  putstring_nl("Ready!");
}

void loop()
{
  if(digitalRead(buttonPin) == LOW)
  {
    int value = analogRead(potPin);
    int index = map(value,0,1023,0,8); // index into one of the 9 files
    playcomplete(wavFiles[index]);
    Serial.println(value);
  }
}

// Plays a full file from beginning to end with no pause.
void playcomplete(char *name)
{
  // call playfile find and play this name
  playfile(name);
  while (wave.isplaying) {
    // do nothing while it's playing
  }
  // now it's done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) { 
  // already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!file.open(root, name)) {
    putstring("Couldn't open file ");
    Serial.print(name);
    return;
  }
  // read the file and turn it into a wave object
  if (!wave.create(file)) {
    putstring_nl("Not a valid WAV");
    return;
  }
  // start playback
  wave.play();
}

void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1)
    ; // stay here if there is an error
}
      
    
