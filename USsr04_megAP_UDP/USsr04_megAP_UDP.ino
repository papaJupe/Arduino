/*  USsr04 megAP UDP -- US sr04 module on Mega w/ ESP shield, send bytes
    over wifi UDP to prog like Proc on PC, can view Ardu's I/O w/ ser. mon.

  mod from example libs/WifiEsp/UdpSendReceive.ino and USsr04 mega UDP TX.ino
  and ESPwf shield AP to be AP w/ udp instead of using LAN AP;

  Ardu w/ US data--> wifi shield as AP+UDP--> sends UDP--> prim ESP w/ USB ser
  onboard Huzz-Feat --> iMac/PC Ser Mon, code ESPrimUDPtest; or Proc w/ ser
  --> SerialDataReadGraph, not tried yet
  
  2 way comm works fine, but packet I/O w/ udp @ both ends needs custom
  config, far from transparent serial from Ardu to Proc or LV
  
  Uses: Ardu mega gets dist_avg using exp MA of pulseIn, sends w/ Ser1,2
  to ESP shield (AP+UDP comm) sends data as UDP pkt to remote ESP Huzz-Feat
  tethered w/ USB on PC/mac w/ Proc2 app or Ser Mon

  Wire MEGA: serial2 pins connect to ESP shield's rx/tx, gnd D15;
    TX=16 (voltage div.-->3.3v), RX=17, to RX/TX of shield
  Wire US sr04:
   Vcc pin to +5v from pin 11    // via shield pass thru
   Trig pin to digital pin 12
   Echo pin to digital pin 13 -- non-interrupt pin works OK
   gnd to pin 13+1

*/

#include <elapsedMillis.h>  // to Ser.print / send upd at interval
#include <WiFiEsp.h>  // enable wifi comm via shield
#include <WiFiEspUdp.h>

// Emulate Serial2 on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial2(6, 7); // RX, TX
#endif

char ssid[] = "espShe";        // AP's SSID (name) 2.4ghz only
char pass[] = "twerpton";   // AP's password
int state = WL_IDLE_STATUS;    // Wifi radio status, AP later
int reqCount = 0; // # of requests incoming

unsigned int localPort = 8888;  // local port shield listens to, any?
unsigned int remotePort = 2390;  // set in remote sketch
IPAddress remoteIp(192,168,4,3); // enter manually what it got from AP

char packetBuffer[44];  // buffer for incoming pct <- size must be < this
char replyBuffer[] = "ACK";      // acknowl string to send back

#define CM 1      //Centimeter, if 1 loop displays Cm, if 0 displays Inch
#define TP 12     //Trigger pin
#define EP 13     //Echo pin

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 1000;  // ms for local ser mon printing prn

// WiFi object is self instantiated by lib, this isn't
WiFiEspUDP Udp;

void setup()
{
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);  // ground for level shift on Ser2 TX
  Serial.begin(115200);    // speed for Ser. Mon. if on for debug
  Serial2.begin(115200); // must = wifi shield's, soft ser needs slower
  pinMode(TP, OUTPUT);   // TP output pin for trigger
  pinMode(EP, INPUT);     // EP input pin for echo
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH); // 5 v for US module power
  //init ESP shield
  WiFi.init(&Serial2);  // ser2 rx/tx pins comm w/ shield
  delay(200);
  // check for presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to LAN WiFi network
//  while (status != WL_CONNECTED)
//  {
//    Serial.print("Try connect to WPA SSID: ");
//    Serial.println(ssid);
//    // Connect to WPA/WPA2 network
//    status = WiFi.begin(ssid, pass);
//  }

  Serial.println("Attempt to start AP");

   // uncomment these two lines to set the IP addr of AP
 // IPAddress localIp(192, 168, 4, 1); // <- default if you don't set
 // WiFi.configAP(localIp);   // optional to set IP

  // start access point
  state = WiFi.beginAP(ssid, 10, pass, ENC_TYPE_WPA2_PSK);
  delay(100);
  // Serial.println("WiFi.begin state: " + String(state));
  if (state)  printWifiStatus();
    else {
      Serial.println("AP init failed; reboot?\n");
      while(1);
         }

  // init UDP service
  Serial.println("\nTry enable UDP service ...");
  // if you get udp service, it rePorts to serial mon.
  Udp.begin(localPort);  // set manually above
  delay(500);    // this starts .listen
  Serial.print("Listening on port ");
  Serial.println(localPort); // remote app must match this for comm

}  // end setup

void loop()
{
  // loop runs 10 hz, reads US ping data, sends it when gets UDP req
  // udp in response mode, remote device can init response or this can
  static uint16_t distAvg = 690; // need to start somewhere, anywhere

  // if there's incoming, [read a packet and] send ack, distAvg, or not
  int packetSize = Udp.parsePacket(); // pkt size recd
  if (packetSize)
  { // mainly for debug, don't need to print this if comm OK
    Serial.print("Received packet size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.println(remoteIp); 
    Serial.print("Port ");
    Serial.println(Udp.remotePort()); // ESPDrvlib edited, now see 4 digits

    // read the packet into packetBufffer, should clear it as well?
    int len = Udp.read(packetBuffer, packetSize); // ch ascii vals sent
    // Udp.read(packetBuffer, packetSize);  // but pktBuff is ch[]
    if (len > 0 && len < 44) // must be < than array size
      packetBuffer[len] = 0;  // put null at end of data; ? now a string
    Serial.print("length: ");
    Serial.println(packetSize);
    Serial.print("Contents: ");
        // chars from remote; string print stops @ null?
    Serial.println(packetBuffer); // ch[] prints chars as string correctly
     // if byte array, .write should print ch, .prints ascii vals
    //for (byte b=0; b<packetSize; b++) Serial.print(char(packetBuffer[b]));
    Serial.println(" ------- ");  // break between incoming

   memset(packetBuffer, 0, 44);  // ? needed
  // send reply to IP addr & port that sent packet (can manually enter)
  // Udp.remotePort() was not read correctly, fixed in EspDrv cpp lib
   //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort() ); // should work now
    Udp.beginPacket(remoteIp, remotePort );  // set manually above
    Udp.write(replyBuffer, sizeof(replyBuffer)); // byte(#), array(name,size)
    Udp.endPacket();    // sends 'ack'
    
    // send US data on any incoming, can refine to respond more specifically
  // second pkt was sent but not seen if remote loop too slow, so put below
//    byte dis[2];  // break distance into hi/lo bytes
//    dis[0] =  (distAvg / 128); // hi byte -- Proc byte max val is 127
//    dis[1] =  (distAvg % 128); // lo byte -- so that's max we can send
//    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//    Udp.beginPacket(remoteIp, remotePort );
//    Udp.write(dis, 2);
//    Udp.endPacket();
//    Serial.println(distAvg);  // ser. mon. can show what's sent

  }  // end if pktsiz, response to incoming data
    // can just send distance on some interval, as in timeElaps below
    
  // getting US data, runs each loop ~10 hz whether prompt incoming or not
  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // how long to wait for value? v.i.--this is OK up to 300cm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;

  // print to ser. mon./send udp at some sane interval prn
    if (timeElapsed > interval)
      { 
//        Serial.print("dist : ");
//        Serial.print(distAvg);
//        if (CM) Serial.println(" mm");
//        else Serial.println(" in");
//        //  lcd.setCursor(0, 1);
//        //  lcd.print("uS: ");
//        //  lcd.print(microseconds);
//        // Serial.println(distAvg);  // for ser. mon.
      byte dis[2];  // break distance into hi/lo bytes
      dis[0] =  (distAvg / 128); // hi byte -- Proc byte max val is 127
      dis[1] =  (distAvg % 128); // lo byte -- so that's max we can send
      //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.beginPacket(remoteIp, remotePort );
      Udp.write(dis, 2);
      Udp.endPacket();
      timeElapsed = 0;
      }  // end time elapsed

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

uint16_t Distance(uint32_t time, int flag) // return distance as unsigned int
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
  // print SSID of this softAP:
  Serial.print("my SSID: ");
  Serial.println(ssid);

  // print (this) WiFi shield's IP address (default or set above)
  IPAddress ip = WiFi.localIP();
  Serial.print("my IP Address: ");
  Serial.println(ip);

  // print the received signal strength: (fails, may not exist for AP)
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm");
}  // end printWiFistatus
