
void setup() {
  Serial1.begin(115200);
}
void loop() {
  Serial1.println("Hello! - millis() = " + String(millis()));
  delay(500);
}
