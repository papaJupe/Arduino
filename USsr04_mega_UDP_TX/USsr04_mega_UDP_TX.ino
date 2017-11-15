/* USsr04 mega UDP TX -- US sr04 module on Mega w/ ESP shield, send bytes
    over wifi UDP to prog like Proc on PC, also view Ardu's I/O w/ ser. mon.

  US data on Ardu--> wifi shield sends UDP--> wifi AP (can be on PC)--> 
  Proc running UDP lib --> SerialDataReadGraph

  Uses: gets dist_avg using exp MA of pulseIn, udp .write bytes, Ser2 to shield
  for comm w/ remote PC, mega's TX=15 (voltage divider-->3.3v), RX=16, to RX/TX
  on shield, AP on iMac, v.s. Proc2 app

  Setup for MEGA: serial2 pins connect to ESP shield I/O
   Vcc pin to  +5 from pin 11    // via shield pass thru
   Trig pin to digital pin 12
   Echo pin to digital pin 13 -- non-interrupt pin works OK
   gnd to pin 13+1
   Serial Mon. sees data I/O prn
*/

#include <elapsedMillis.h>  // to .print at interval
#include <WiFiEsp.h>  // enable comm via shield
#include <WiFiEspUdp.h>

// Emulate Serial1 on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "hny_Trp";        // your network SSID (name) 2.4ghz only
char pass[] = "hunniBunch69";   // your network password
int status = WL_IDLE_STATUS;    // the Wifi radio's status

unsigned int localPort = 8888;  // local port to listen on, any unused

char packetBuffer[255];          // buffer to hold incoming packet
byte replyBuffer[] = "ACK";      // acknowl string to send back

#define CM 1      //Centimeter, if 1 loop displays Cm, if 0 displays Inch
#define TP 12     //Trigger pin
#define EP 13     //Echo pin

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 1000;  // ms for ser mon printing prn

WiFiEspUDP Udp;

void setup()
{
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistors
  Serial.begin(9600);    // speed for Ser. Mon. if on
  Serial2.begin(115200); // must = wifi shield's, soft ser needs slower
  pinMode(TP, OUTPUT);   // TP output pin for trigger
  pinMode(EP, INPUT);     // EP input pin for echo
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH); // 5 v for US module power
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
  Serial.println(localPort); //from AP by DHCP?; # in sending app must match

}  // end setup

void loop()
{
  // loop runs 10 hz, getting US ping data, sends latest when asked by UDP

  static uint16_t distAvg = 690; // need to start somewhere, anywhere

  // if there's data available, [read a packet and] send latest distAvg
  int packetSize = Udp.parsePacket();
  if (packetSize)
  { // mainly for debug, don't need to see if comm OK
    Serial.print("Received packet size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort()); //only last 3 digits, don't work below

    // read the packet into packetBufffer, does this clear as well?
    int len = Udp.read(packetBuffer, 255);
    if (len > 0)
      packetBuffer[len] = 0;  // puts a null at end of data; ? now a string
    Serial.println("Contents:");
    Serial.println(packetBuffer);  // chars from remote; ? stops @ null ?

    // send reply to IP addr & port (manually enter #) that sent the packet
    // Udp.remotePort() not read correctly, so I added # from Proc sketch
    //    Udp.beginPacket(Udp.remoteIP(), 8884 );
    //    Udp.write(replyBuffer, sizeof(replyBuffer)); // byte(#), array(name,size)
    //    Udp.endPacket();
    
    // send US data on any incoming, can refine to respond more specifically
    // Udp.write(replyBuffer, sizeof(replyBuffer)); byte(#), array(name,size)
    byte dis[2];
    dis[0] =  (distAvg / 128); // hi byte -- Proc byte max is 127
    dis[1] =  (distAvg % 128); // lo byte -- so that's max we can send
    Udp.beginPacket(Udp.remoteIP(), 8884);
    Udp.write(dis, 2);
    Udp.endPacket();
    Serial.println(distAvg);  // ser. mon. can show what's sent

  }  // end if pktsiz,  response to incoming data

  // getting US data, runs each loop anyway ~ 10 hz
  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // how long to wait for value? v.i.--this is OK up to 300cm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;

// print to ser. mon. at some sane interval prn
//  if (timeElapsed > interval)
//    { Serial.print("dist : ");
//      Serial.print(distAvg);
//      if (CM) Serial.println(" mm");
//      else Serial.println(" in");
//      //  lcd.setCursor(0, 1);
//      //  lcd.print("uS: ");
//      //  lcd.print(microseconds);
//      // Serial.println(distAvg);  // for ser. mon.
//      timeElapsed = 0;
//    }

  delay(80);
}  // end loop

uint32_t TP_init()  // trigger sensor, get pulse width back
{
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  digitalWrite(TP, HIGH); // make Trig pin HIGH >= 10us
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  // pulse read waits for the echo pin to go HIGH, and
  // returns the length of the echo pulse in microseconds
  uint32_t microseconds = pulseIn(EP, HIGH);
  return microseconds;
}

uint16_t Distance(uint32_t time, int flag)  // returns distance as unsigned int
{
  uint16_t distance;  // SR04 only works to ~3m, so max time < 18K uS
  if (flag) // display in cm./1000 or mm /100
    distance = (time * 17) / 100; // use mm to demo sending large #
  // distance  = ((Duration of high level)*(Sonic:340m/s))/2
  // = ((Duration of high level)*(Sonic :0.034 cm/uS))/2
  else  // 1122 ft/sec   13464 in/sec   0.0135 in/uS / 2 for bounce
    distance = (time * 67) / 10000;       // INches
  return distance;
}  // end Distance


void printWifiStatus() {
  // print SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
