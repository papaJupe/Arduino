/*
  Interrupts sketch
  see Recipe 10.1 for connection diagram
 */

const int irReceiverPin = 2;         //pin the receiver is connected to
const int numberOfEntries = 64;      // set this number to any convenient value        

volatile unsigned long microseconds;
volatile byte index = 0;
volatile unsigned long results[numberOfEntries];

void setup()
{
  pinMode(irReceiverPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, analyze, CHANGE);  // encoder pin on interrupt 0 (pin 2);
  results[0]=0;
}

void loop()
{
  if(index >= numberOfEntries)
  {
    Serial.println("Durations in Microseconds are:") ;
    for( byte i=0; i < numberOfEntries; i++)
    {
      Serial.println(results[i]);
    }
    index = 0; // start analyzing again
  }
  delay(1000);
}

void analyze()
{
  if(index < numberOfEntries  )
  {
    if(index > 0)
    {
      results[index] = micros() - microseconds;
    }
    index = index + 1;
  }
  microseconds = micros();
}
      
    
