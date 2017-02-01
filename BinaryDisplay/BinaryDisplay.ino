// BinaryDisplay -- display an arbitrary # in binary lites; could  
// get random # or convert a serial or switch input to binary, etc

#define flag 14  //just some arbitrary number <16; could be from some input or data
 
byte pinOuts[]= {9,10,11,12};  //  these are [0,1,2,3] output pins
                               //        for 1s,2s,4s,8s
void setup() 
{
  //  code here runs once:
  Serial.begin(9600);
  pinMode(9,OUTPUT);   // lowest bit of #
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT); // 8's bit of # displays here
}

void loop() //   // main code here runs repeatedly
{
 
  for (byte i=0; i < 4; i++)
     { 
      byte pin = pinOuts[i];
      if (bitRead(flag, i) == 1) // if bit i of flag is 1 make respective
                                      // binary output pin high  
         digitalWrite(pin, HIGH);
      else
         digitalWrite(pin, LOW);
     }
  Serial.print("flag is now: ");
  Serial.println(flag);
  delay(1000);
}
