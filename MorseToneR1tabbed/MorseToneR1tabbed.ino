/* Monk's sketch 5-05 morse reader v 1: tab mod shows use of new tab to hold fx
 I modified to play tone thru tonePin;
 enter char string into serial monitor, plays tone and led flashes,
 delay 4 sec before each new char string plays; then added
 more code to make it play random strings if switch tells it to;
 consider adding pot control of speed, serial readout of char/min and 
 what was played to test accuracy; Rand2 added serial readout and simpler
 construction of random words
 */
 
#define ledPin 13
#define dotDelay 160    // all speed params derive from this value, could make adjustable
#define tonePin 8
                         // char*name[] makes elements of name[] be pointers? (* spacing irrelevant)
char * letters[] = {  // elements of array look like string constants (actually char arrays?)
    // but what letters[] actually holds are pointers to the arrays
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};

char*numbers[] = {      //  * spacing irrelevant, but it's type def for char not deref numbers[]
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

void setup()                 
{   
  pinMode(A0, INPUT);  // (just for clarity, defaults to this) loose wire here gives floating voltage for random
  pinMode(15, INPUT);  // A1 will be high unless switch pulls it low, then makes randomized code
  digitalWrite(15, HIGH);  // could use INPUT_PULLUP in pinMode instead
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
}

void loop()                    
{
  char ch;   // to hold indiv char to play from serial input
  char words[8]; // to hold 'words' of 7 random numbers plus a space
  static boolean var=0;  // just set it once when beginning
                       // otherwise if/while condx will delay each char
  if (Serial.available() == 0) // will remain > 0 while character in buffer;
           
       var=0;               // resets this to 0 when buffer empties again
       
  if (Serial.available() > 0)
  
  {                            // if/while condition pauses 3 sec
        if (var == 0)      // after new serial data comes
        {                    // before starting output
        delay(3000);
        var = 1;
        }
      ch = Serial.read();   //get char one by one from buffer, will be ascii value of char
  
      if (ch >= 'a' && ch <= 'z')      // 'z' = ascii value of z
      {    
        flashSequence(letters[ch - 'a']);  // so this gives location of ch in letter array
      }
      else if (ch >= 'A' && ch <= 'Z')
      {
        flashSequence(letters[ch - 'A']); // flashing fx in their own tab
      }
      else if (ch >= '0' && ch <= '9')
      {
        flashSequence(numbers[ch - '0']);
      }
      else if (ch == ' ')
      {
        delay(dotDelay*4);  // gap between words  
      }
  }  //end if Ser.avail
    
  if (digitalRead(15) == LOW)   // switch is closed to activate random code
  { 
    int floteV = analogRead(0);    // should range 0 to 1024
    randomSeed(floteV);
    Serial.print("seed is: "), Serial.println(floteV); //used to debug
    
    for (byte i=0; i<7; i++)   // make 7 character 'words' of random #s
     {                        // random must be inside 'for' block to give you a sequence
 
       words[i] = random(30);  // put random # 0-29 in i slot of char array words
       Serial.print(words[i],DEC);   // must use DEC to show as #, used to debug
       Serial.print(" ");            // show array elements on 1 line
     }
     Serial.println();  // blank line
     words[7] = 32;    // puts space at end of word array
                               
     for (byte i=0; i<8; i++)   //convert random # words to char's, flash, and print them
     {
        ch = words[i]; 
        if (ch <= 25)  // one of the 26 letters
        {    
          flashSequence(letters[ch]); // eg letters[1] will be param for flashSequence
                                      // to use as pointer to char string for b
        }
        
        else if (ch >= 26  && ch <= 29)  //  4 slots that designate numbers
        {
          byte randY = random(0,256); // random(0,9) gave mostly 5
          randY = (randY % 10);   //want a random digit 0-9, hard getting it
          Serial.print("randY is: "), Serial.println(randY); // for debugging
          flashSequence(numbers[randY]);
        }
        else if (ch == 32)  // end of random array, make 'space', simulating word
        {
          delay(dotDelay*4);  // gap between words  
        }
     }  // end for display
  } // end if for random generation
}   // end loop

