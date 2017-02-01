/*
   ForLoop sketch
   demonstrates for loop
*/

void setup() {
  Serial.begin(9600);}

void loop(){
  Serial.println("for(int i=0; i < 4; i++)");
  for(int i=0; i < 4; i++)
  {
    Serial.println(i);
  }
}
      
    
