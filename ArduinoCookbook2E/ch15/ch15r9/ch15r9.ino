/*
 * WebServerMultiPageHTML
 * Arduino 1.0 version
 * Display analog and digital pin values using HTML formatting
 */

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] =  { 192,168,1,177 };

// Buffer must be big enough to hold requested page names and terminating null
const int MAX_PAGE_NAME_LEN = 8+1;  // max characters in a page name + null
char buffer[MAX_PAGE_NAME_LEN]; 

EthernetServer server(80);
EthernetClient client;

void setup()
{
  Serial.begin(9600);  
  
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(13,OUTPUT);
  for(int i=0; i < 3; i++)
  {
     digitalWrite(13,HIGH);
     delay(500);
     digitalWrite(13,LOW);
     delay(500);
  }
}

void loop()
{
  client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        if( client.find("GET ") ) {
          // look for the page name
          memset(buffer,0, sizeof(buffer)); // clear the buffer
          if(client.find( "/"))
            if(client.readBytesUntil('/', buffer, MAX_PAGE_NAME_LEN )) 
            {
              if(strcasecmp(buffer, "analog") == 0)
                showAnalog();
              else if(strcasecmp(buffer, "digital") == 0)
                showDigital();
              else
                unknownPage(buffer);
            }
        }
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}

void showAnalog()
{
  sendHeader("Multi-page: Analog");
  client.println("<h2>Analog Pins</h2>");
  client.println("<table border='1' >");
  for (int i = 0; i < 6; i++) {
    // output the value of each analog input pin
    client.print("<tr><td>analog pin ");
    client.print(i);
    client.print(" </td><td>");
    client.print(analogRead(i));
    client.println("</td></tr>");
  }
  client.println("</table>");
  client.println("</body></html>");
}

void showDigital()
{
  sendHeader("Multi-page: Digital");
  client.println("<h2>Digital Pins</h2>");
  client.println("<table border='1'>");
  for (int i = 2; i < 8; i++) {
    // show the value of digital pins
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // turn on pull-ups
    client.print("<tr><td>digital pin ");
    client.print(i);
    client.print(" </td><td>");
    if(digitalRead(i) == LOW)
      client.print("Low");
    else
      client.print("High");
    client.println("</td></tr>");
  }
  client.println("</table>");
  client.println("</body></html>");
}

void unknownPage(char *page)
{
  sendHeader("Unknown Page");
  client.println("<h1>Unknown Page</h1>");
  client.print(page);
  client.println("<br />");
  client.println("Recognized pages are:<br />");
  client.println("/analog/<br />");
  client.println("/digital/<br />"); 
  client.println("</body></html>");
}

void sendHeader(char *title)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.print("<html><head><title>");
  client.println(title);
  client.println("</title><body>");
}  
