/* Monk's sketch 5-05 morse reader, modified to play tone thru pin toneOut;
 enter char string into serial monitor, plays morse equivalent by tone and led,
 delay 3 sec before each new char string plays; then added
 more code to make it play random strings if switch is closed;
 Rand1 sketch made an array using for, then played that in another for loop;
 Rand2 is simpler and adds ser. mon. readout of each 7 letter word;
 Rand3 added pot control of speed; later add serial readout of char/min ?
*/
 
#define potPin 2       //reading pot voltage here, will set speed
#define toneOut 8 
#define ledPin 13
#define analRand 18  // A4 picks up stray voltage hopefully
int dotDelay = 150;   // all speed params derive from this value, adjustable w/ pot

char*letters[] = {    // letters[] will contain pointers to char strings
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};

char * numbers[] = {  // * spacing irrelevant, but char* is type def not deref of numbers[]
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void setup()                 
{   
  pinMode(analRand, INPUT);  //makes no difference if here or not, wire gets floating voltage for random char pick
  pinMode(14, INPUT);  // 14 is same as A0, will be high unless switch pulls it low
  digitalWrite(14, HIGH);  // could use INPUT_PULLUP instead
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
}

void loop()                    
{
  int potRead = analogRead(potPin);
  dotDelay =map(potRead,5,1020,220,70);  // higher volts makes smaller delay = faster
  char ch;                            // to hold indiv char to play from serial input
  static boolean var=0;            // just set it once when beginning
                                   //otherwise if/while condx will delay each char
  if (!Serial.available())  // nothing in the buffer
       {    
       var=0;      // ser.avail will be > 0 while characters can be read from buffer;
                   // this resets var to 0 when buffer empties again
       }
  if (Serial.available())   //same as (Ser.avail() > 0), value = # of bytes being held in buffer
  
   {                     // if/while condition pauses 3 sec on first loop and
      if (var == 0)      // after new pack of serial data comes
      {                  // before starting output
      delay(3000);
      var = 1;
      }
      ch = Serial.read();   //get char one by one from buffer, will be ascii value of char
  
      if (ch >= 'a' && ch <= 'z')           // 'z' is ascii value of z
      {    
        flashSequence(letters[ch - 'a']);  // so this gives index of ch in letter array
      }
      else if (ch >= 'A' && ch <= 'Z')
      {
        flashSequence(letters[ch - 'A']);   //actually sending a pointer (address) to the function
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
    
  if (digitalRead(15) == LOW)   // switch is closed to activate random word generation
  { 
    static boolean var = 0;  // just read it once, so
                                     // random generator pauses 3 sec
    if (var == 0)            // before starting output
    delay(3000);
    var = 1;
    int floteV = analogRead(analRand);    // should range 0 to 1023, actually doesn't vary a lot
    randomSeed(floteV);           //each 'for' block below will get new seed for each word
    //    Serial.print("seed is: "), Serial.println(floteV); //used to debug
  
    for (byte i=0; i<7; i++)   // each run makes 7 character word from random #s
    {                        // random must be inside 'for' block to give you a sequence ?
 
       char ch = random(30);  // put random # 0-29 in ch
       
       if (ch <= 25)  // one of the 26 letters
        {    
          flashSequence(letters[ch]);   //  pointer to a char string passed to function
          Serial.write(ch + 'A'), Serial.write(32);  // print the char + space
        }            // to print capital letter A-Z must add ascii value of A to ch value
                      // ser.print just prints the ascii # instead of its char value
                      // not sure why since ch is char type; maybe adding 'A' makes it an int?
                      
        else if (ch >= 26  && ch <= 29)  //  4 random slots for designating numbers
        {
          byte randY = random(0,256); //random(0,9) gave mostly 5
          randY = (randY % 10);   //want a random digit 0-9, hard getting it
          // Serial.print("randY is: "), Serial.println(randY); //for debugging
          flashSequence(numbers[randY]); //numbers[3] sends pointer to morse 4 sequence to flashSeq.          
          Serial.print(randY), Serial.write(32);  // print the # + space; Ser.print(32) prints 32
        }  // end else                        //randY is a byte so .print shows its numeric value
    }  // end 'for' word generation               
       delay(dotDelay*4);  // gap between 7 letter words
       Serial.println();  // new line for each word
  } // end if switch on, random generation
}   // end main loop
  
  void flashSequence(char * sequence)  // I think * is part of type def of seq., not deref of it
                    /* char* defines param 'seq.' as a pointer (an address)
                  that will hold values of type char, in this case, a string literal */
  {
    int i = 0;                 //read thru char array until NULL reached
    while (sequence[i] != NULL)
    {                      //each i sends a dot or dash to flasher/toner
      flashDotOrDash(sequence[i]);
      i++;
    }
    delay(dotDelay*3);    // gap between letters
  }

void flashDotOrDash(char dotOrDash)
{
   digitalWrite(ledPin, HIGH);
   tone(toneOut, 2300);   // don't seem to need pin config
    if (dotOrDash == '.')
    {
      delay(dotDelay);           
    }
    else // must be a dash [any char not a . plays a dash]
    {
      delay(dotDelay * 3);           
    }
    digitalWrite(ledPin, LOW);
    noTone(toneOut);  
    delay(dotDelay); // gap between flashes
}   // end function


