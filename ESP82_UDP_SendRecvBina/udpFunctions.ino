// connect to UDP - returns true if successful or false if not
  boolean connectUDP(){
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");
  
  if(UDP.begin(localPort) == 1){
    Serial.println("Connection successful");
    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  
  return state;
}
