/* ESP82shield mega UDP tx rx --  Mega w/ ESP shield, send / recv bytes
   over wifi UDP to prog like Proc on PC, also view Ardu's I/O w/ ser. mon.
   mod from UDPsendreceive in wifiESP examples

  [US data on Ardu--> wifi shield sends UDP--> wifi AP (can be on PC)-->]
  Proc running UDP lib --> sends control bytes to Ardu for servo; first got
  a byte for up/down and increment tilt var; this adds getting an int
  from a slider from UDP on Proc, or joystick button up/down control

  Uses: udp read /.write bytes, Ser2 to shield, servo.write, convert
  incoming data to int angle for servo, elapsedMillis for timed print,
  
  for comm w/ remote PC, mega's TX=16 (voltage divider-->3.3v), RX=17,
  to RX/TX on shield, send to Proc2 app, AP on iMac, or separate AP router

   Serial Mon. sees data I/O prn
*/
# include <Servo.h>
#include <elapsedMillis.h>  // to .print at interval
#include <WiFiEsp.h>  // enable comm via shield
#include <WiFiEspUdp.h>

// Emulate Serial1 on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

//char ssid[] = "hny_Trp";        // your network SSID (name) 2.4ghz only
//char pass[] = "hunniBunch69";   // your network password
char ssid[] = "dap15";        // free standing router works too
char pass[] = "hunniBranch69";

int status = WL_IDLE_STATUS;    // the Wifi radio's status

unsigned int localPort = 8888;  // local port to listen on, any unused #

char packetBuffer[10];     // buffer to hold incoming packet, can't be small
// byte replyBuffer[] = "ACK";   // acknowl string to send back, char/byte?
byte replyBuffer[] = "30";  // must be byte array ?<- string, final null sent too

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 1500;  // ms for ser mon printing prn

WiFiEspUDP Udp;
Servo tiltServ;
int posiT = 90; // default neutral position, or motor stop

void setup()
{
  // on mega servo pwr must come from VR not D pins, unclear why
  tiltServ.attach(2);  // signal for servo on D2

  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistor

  Serial.begin(9600);    // speed for Ser. Mon. if on
  Serial2.begin(115200); // must = wifi shield's, soft ser needs slower
  tiltServ.write(posiT);  // init to neutral angle
  //init ESP shield
  WiFi.init(&Serial2);  // ser2 rx/tx pins comm w/ shield

  // check for presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED)
  {
    Serial.print("Try connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nTry connect to remote UDP service...");
  // if you get a connection, report back via serial mon.:
  Udp.begin(localPort);
  delay(500);
  Serial.print("Listening on port ");
  Serial.println(localPort); // # in sending app must match

}  // end setup

void loop()
{
  // loop runs 10 hz, gets control bytes from Proc/wifi-UDP/shield/Ardu

  // if there's data available, read a packet and send ? something back
  int packetSize = Udp.parsePacket();
  if (packetSize)
  { // print for debug prn
    IPAddress remoteIp = Udp.remoteIP();
    int remoteP = Udp.remotePort();        
//    Serial.print("Received packet size ");
//    Serial.println(packetSize);
//    Serial.print("From ");
//    Serial.print(remoteIp);
//    Serial.print(", port ");
//    Serial.println(remoteP); //see only last 3 digits, really 4

    // read the packet into packetBufffer, does this clear as well? Y
    int len = Udp.read(packetBuffer, packetSize);
    if (len > 0)  // puts a null at end of data; ? char[] same as string
      packetBuffer[len] = 0;
    Serial.print("Contents:");
    Serial.println(packetBuffer);  // chars from remote, stops @ null

    // udp reply to IP addr & port (manually enter #) that sent the packet
    // Udp.remotePort() not read correctly, enter # from Proc sketch
//    Udp.beginPacket(Udp.remoteIP(), 8884 ); // ack optional, 4 debug prn
//    Udp.write(replyBuffer, sizeof(replyBuffer)); // byte[], its size)
//    Udp.endPacket();

    // parse pkt and change servo setting posiT within tilt limits
    // math looks reversed because 40 is full up, 150 full down tilt
    // seems like 'key' bytes still work as second byte of string
    // but only if draw loop is not sending # continuously
    if (packetBuffer[1] == 'u' && posiT > 40) posiT -= 10;
    else if (packetBuffer[1] == 'd' && posiT < 150) posiT += 10;
    else   // parse string input to int, and send to servo as posiT
    {   // possibly use udp.parseInt()?
      int posiPoss = atoi(packetBuffer);  // conv buffer char[] to int
      if ((posiPoss - 40) >= 1 && (posiPoss - 40) <= 109)
        posiT = posiPoss;  // in 41-149 range, use the new value
    }

  }  // end if pktsiz, respond to incoming data

  tiltServ.write(posiT); // write current # to servo
  // print to ser. mon. at some sane interval prn
  if (timeElapsed > interval)
  {
    Serial.print("angle: ");
    Serial.println(posiT);
    timeElapsed = 0;
  }

  delay(100);
}  // end loop

void printWifiStatus() {
  // print SSID of the network/AP you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address: (got from DHCP)
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
