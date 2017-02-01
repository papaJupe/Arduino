/*
 * Web Server
 * simple web server using Stream parsing.
 */

#include <SPI.h> 
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177};  // IP address of this web server

EthernetServer server(80);

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
         if( client.find("\r\n\r\n")) { // find the blank line indicating end of HTTP request
            // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          // output the value of each analog input pin
          for (int i = 0; i < 6; i++) {
            client.print("analog input ");
            client.print(i);
            client.print(" is ");
            client.print(analogRead(i));
            client.println("<br />");
          }
          break;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}
