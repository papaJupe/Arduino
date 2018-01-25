///////////////////////////////////////////////////////////////////////////////
//                             //
//              Lady Buggy       from instructables project, small bug w
//            2 servos controlled by ESP82 prim device Feather Huzzah
         and iphone browser -- needs WSS lib installed (manager), much tune up
////////////////////////////////////////////////////////////////////////////////////////

#define USE_SERIAL  1

////////////////////////////////////////////////////////////////////////////////////////
//
// Library includes.
//
/////////////////////////////////////////////////////////////////////////////////////////

      #include    <Adafruit_FeatherOLED.h>         // for the Adafruit Featherwing oled
      #include    <ESP8266WebServer.h>      // for the html web server (web page)
      #include    <ESP8266mDNS.h>           // for MDNS
      #include    <WebSocketsServer.h>      // for the socket server (data send) need to add
      #include    <Servo.h>          // for servo drive
      
////////////////////////////////////////////////////////////////////
//
// Constants.
//
///////////////////////////////////////////////////////////////////////////
  // Buttons on Oled.
  
      #define     BUTTON_A   0       // button A pin
      #define     BUTTON_B   16      // button B pin
      #define     BUTTON_C   2       // button C pin
 
 // Oled display.

      // Dimensions.
      
      #define     DISPLAY_HEIGHT    32      // height of afOled display in pixels
      #define     DISPLAY_WIDTH     128     // width of afOled display in pixels

      // Font dimensions (estimated).
      
      #define     FONT_ONE_HEIGHT   8       // height of font one character
      #define     FONT_ONE_WIDTH    6       // width of font one character
      #define     FONT_TWO_HEIGHT   14      // height of font two character
      #define     FONT_TWO_WIDTH    12      // width of font two character

  // Port numbers.

      #define     PORT_HTTP  80      // port number for http
      #define     PORT_WS    2205    // port number for socket
      
  // Servo.
  
      #define     SERVO_OFF  90      // value to stop servo
    
/////////////////////////////////////////////////////////////////////
//
// Variables.
//
////////////////////////////////////////////////////////////////
     
  // Buttons.

      static  int        nButtonA = 0;      // button a pressed (1 = pressed)
      static  int        nButtonB = 0;      // button b pressed (1 = pressed)
      static  int        nButtonC = 0;      // button c pressed (1 = pressed)
     
  // Oled.
    
      Adafruit_FeatherOLED      afOled = Adafruit_FeatherOLED();        // oled display

  // Servos.
  
      Servo       ServoLeft;         // left servo
      Servo       ServoRight;        // right servo

  // Wifi.

    // Acccess point ssid.
    
      IPAddress          IpAddress(192,168,20,20);        // access point ip address
      IPAddress          IpGateway(192,168,20,1);         // access point gateway
      IPAddress          IpSubnet(255,255,255,0);         // access point subnet mask
      String      sSsidLadyBuggygy = "LadyBuggygy";       // access point ssid

    // Servers (http and sw).

      int         nX = 50;           // servo x
      int         nY = 50;           // servo y
      ESP8266WebServer   wsWebServer(PORT_HTTP);          // html server
      WebSocketsServer   webSocket = WebSocketsServer(PORT_WS);  // socket server
      
    // Wifi router password, wifi router ssid and access point ssid.
    
      String      sPassword = "your_router_password";     // router and access point password
      String      sSsidAccessPoint = "LadyBuggy";  // access point ssid
      String      sSsidRouter = "your_router_ssid";       // router ssid

///////////////////////////////////////////////////////////////////////////////////
//
//  Lady Buggy web page (client).
//
//////////////////////////////////////////////////////////////////////////////

      // The Lady Buggy web page string consists of both Javascript (for touch control, touch control graphics
      // and data transmission), and html (for the web page).
      
      String        LadyBuggyWebPage=

   ///////////////////////////////////////////////////////////////////////////////
   //
   // Javascript content of web page string.
   //
   ///////////////////////////////////////////////////////////////////////
   
   "<script type='text/javascript'>\n"

       //
       // Variables.
       //
       
    "var  canvasLadyBuggy;\n"
    "var  canvasLadyBuggyHeight = 0;\n"
    "var  canvasLadyBuggyWidth = 0;\n"
    "var  contextLadyBuggy;\n"
    "var  nLadyBuggyDelay = 0;\n"
    "var  nLadyBuggyPort =" +  String(PORT_WS) + ";\n"
    "var  nLadyBuggyX = 0;\n"
    "var  nLadyBuggyXN1 = -1;\n"
    "var  nLadyBuggyY = 0;\n"
    "var  nLadyBuggyYN1 = -1;\n"
    "var  touchlistLadyBuggy = [];\n"
    "var  wsLadyBuggy = new WebSocket('ws://' + location.hostname + ':' + nLadyBuggyPort + '/');\n"

       //
       // Utilities.
       //

    //
    // sendLadyBuggyXY()
    // Send the nLadyBuggyX and nLadyBuggyX values to the server.
    // Entry   : nothing
    // Returns : nothing
    //
    // Notes   :
    //
    // The server (the lady buggy itself) is expecting the header "zpllc" immediately
    // followed by the x and y touch coordinates in the range of 0 through 99, with
    // leading zeros if necessary.
    
    "function sendLadyBuggyXY()\n"
    "{\n"
        // Convert x and y ints to strings.
        
        "var stringX = String(nLadyBuggyX);\n"
        "var stringY = String(nLadyBuggyY);\n"
    
        // The server is expecting 2 digit values for x and y (00 through 99),
        // so add leading zero(s) to the strings as required.
        
        "while(stringX.length < 2)\n"
            "stringX = '0' + stringX;\n"
        
        "while(stringY.length < 2)\n"
            "stringY = '0' + stringY;\n"
    
        // Combine the strings with the server validation header.
        //
        // Notes:
        //
        //  Again, the server is expecting an ascii string formatted as follows:
        //
        //      "zpllcXXYY"
        //
        //  where:
        //
        //    "zpllc": is a header for server validation of the message string,
        //    "XX"   : is the x value,
        //    "YY"   : is the y value.
        //
        
        "stringX = 'zpllc' + stringX + stringY;\n"
    
        // Send the result to the server.
        
        "wsLadyBuggy.send(stringX);\n"
        
      "}\n" 

    //
    // drawLadyBuggyTouchImage(x, y)
    // Draw the touch image on the canvas.
    // Entry   : x and y location of where to draw the image
    // Returns : nothing
    //
    
    "function drawLadyBuggyTouchImage(x, y)\n"
    "{\n"
        // Start a drawing path.
        
        "contextLadyBuggy.beginPath();\n"

        // Draw a circle.
        
        "contextLadyBuggy.arc(x, y, 40, 0, 2*Math.PI, true);\n"
        "contextLadyBuggy.fillStyle = 'rgba(200, 0, 0, 0.2)';\n"
        "contextLadyBuggy.fill();\n"
        "contextLadyBuggy.lineWidth = 2.0;\n"
        "contextLadyBuggy.strokeStyle = 'rgba(200, 0, 0, 0.8)';\n"

        // Present the drawing path.
        
        "contextLadyBuggy.stroke();\n"

    "}\n"

       //
       // onloadLadyBuggy()
       // Called by the html code when the web page is loaded.
       // Entry   : nothing
       // Returns : nothing
       //
       
       "function onloadLadyBuggy()\n"
       "{\n"
    // Obtain the canvas from the html portion of the web page.
    
    "canvasLadyBuggy = document.getElementById('canvasLadyBuggy');\n"

    // Obtain a drawing context for the canvas.
    
    "contextLadyBuggy = canvasLadyBuggy.getContext('2d');\n"

    // Initialize the canvas.
    
    "canvasLadyBuggyWidth = window.innerWidth;\n"
    "canvasLadyBuggyHeight = window.innerHeight;\n"
    
    "canvasLadyBuggy.style.width = canvasLadyBuggyWidth +'px';\n"
    "canvasLadyBuggy.style.height = canvasLadyBuggyHeight + 'px';\n"
    "canvasLadyBuggy.width = canvasLadyBuggyWidth;\n"
    "canvasLadyBuggy.height = canvasLadyBuggyHeight;\n"

    // Draw the initial touch image.
    
    "drawLadyBuggyTouchImage(window.innerWidth / 2, window.innerHeight / 2);\n"
    
    // Create a function for the "touchstart" event.
    
    "canvasLadyBuggy.addEventListener('touchstart', function(event)\n"
    "{\n"
        // Obtain list of touch points (we only use the first one in Lady Buggy).
        
        "touchlistLadyBuggy = event.touches;\n"

        // Check for existance of a touch point.
        
        "if(touchlistLadyBuggy.length)\n"
        "{\n"
            // Have a touchpoint, update x.

            "nLadyBuggyX = parseInt((100 * touchlistLadyBuggy[0].pageX) / canvasLadyBuggyWidth);\n"

            "nLadyBuggyX = (nLadyBuggyX > 0) ? (nLadyBuggyX < 99) ? nLadyBuggyX : 99 : 0;\n"
            
            // Update y.
            
            "nLadyBuggyY = parseInt((100 * touchlistLadyBuggy[0].pageY) / canvasLadyBuggyHeight);\n"

            "nLadyBuggyY = (nLadyBuggyY > 0) ? (nLadyBuggyY < 99) ? nLadyBuggyY : 99 : 0;\n"

            // Send x and y position data to the server.

         // Update n-1 values.
         
         "nLadyBuggyXN1 = nLadyBuggyX;\n"
         "nLadyBuggyYN1 = nLadyBuggyY;\n"
         "nLadyBuggyDelay = 8;\n"
         
         // Send x, y.
         
         "sendLadyBuggyXY();\n"
            
            // Update the touch image position.
            
            "contextLadyBuggy.clearRect(0, 0, canvasLadyBuggyWidth, canvasLadyBuggyHeight);\n"
            "drawLadyBuggyTouchImage(touchlistLadyBuggy[0].pageX, touchlistLadyBuggy[0].pageY);\n"
            
        "}\n"
    "});\n"
    
    // Create a function for the "touchmove" event.
    
    "canvasLadyBuggy.addEventListener('touchmove', function(event)\n"
    "{\n"
        // No scrolling during touch moves.
        
        "event.preventDefault();\n"

        // Obtain list of touch points.
        
        "touchlistLadyBuggy = event.touches;\n"

        // Check for existance of a touch point.
        
        "if(touchlistLadyBuggy.length)\n"
        "{\n"
            // Have a touchpoint, update x.

            "nLadyBuggyX = parseInt((100 * touchlistLadyBuggy[0].pageX) / canvasLadyBuggyWidth);\n"

            "nLadyBuggyX = (nLadyBuggyX > 0) ? (nLadyBuggyX < 99) ? nLadyBuggyX : 99 : 0;\n"
            
            // Update y.
            
            "nLadyBuggyY = parseInt((100 * touchlistLadyBuggy[0].pageY) / canvasLadyBuggyHeight);\n"

            "nLadyBuggyY = (nLadyBuggyY > 0) ? (nLadyBuggyY < 99) ? nLadyBuggyY : 99 : 0;\n"

            // Send x and y position data to the server.
            //
            // Notes:
            //
            //  In this application, there is no need to continuously send data to the server,
            // so nLadyBuggyDelay is incorporated to send data to the server every 16 passes if
            // either the x or y values have changed.

            "if(((nLadyBuggyXN1 != nLadyBuggyX) || (nLadyBuggyYN1 != nLadyBuggyY)) && ((nLadyBuggyDelay --) <= 0))\n"
            "{\n"
           // x or y has changed and delay is up, update n-1 values and reset delay.
           
           "nLadyBuggyXN1 = nLadyBuggyX;\n"
           "nLadyBuggyYN1 = nLadyBuggyY;\n"
           "nLadyBuggyDelay = 16;\n"
           
           // Send x, y.
           
           "sendLadyBuggyXY();\n"
            "}\n"
            
            // Update the touch image position.
            
            "contextLadyBuggy.clearRect(0, 0, canvasLadyBuggyWidth, canvasLadyBuggyHeight);\n"
            "drawLadyBuggyTouchImage(touchlistLadyBuggy[0].pageX, touchlistLadyBuggy[0].pageY);\n"
            
        "}\n"
    "});\n"

     // Create a function for the "touchend" event.
    
    "canvasLadyBuggy.addEventListener('touchend', function()\n"
    "{\n"
        // At the end of the touch, return to center thus stopping Lady Buggy.

        "nLadyBuggyX = 50;\n"
        "nLadyBuggyY = 50;\n"

        // Send x, y.
        
        "sendLadyBuggyXY();"
        
        // Draw the touch image.
        
        "contextLadyBuggy.clearRect(0, 0, canvasLadyBuggyWidth, canvasLadyBuggyHeight);\n"
        "drawLadyBuggyTouchImage(canvasLadyBuggyWidth / 2, canvasLadyBuggyHeight / 2);\n"
        
     "});\n"
       "};\n"
        
   "</script>\n"
   
   ////////////////////////////////////////////////////////////////////////
   //
   // HTML content of web page.
   //
   /////////////////////////////////////////////////////////////////////////

   "<!DOCTYPE html>\n"
   
   "<html>\n"
   
       "<head>\n"
   
    "<title>Lady Buggy</title>\n"
    "<meta name='viewport' content='width=device-width, user-scalable=no'>\n"
   
       "<style type='text/css'>\n"
       
    "body { margin: 0px; overflow: hidden; }\n"
    "canvas { border: none; }\n"
    
       "</style>\n"
   
       "</head>\n"
       
       "<body onload='onloadLadyBuggy()'>\n"

    "<canvas id='canvasLadyBuggy' width='300' height='300' style='top:0px; left:0px; width: 300px; height: 300px;'></canvas>\n"
      
       "</body>\n"
   
   "</html>"
      ;

///////////////////////////////////////////////////////////////////////////////////////////
// Lady Buggy setup.
//
////////////////////////////////////////////////////////////////////////////////////

void setup() 
{ 
  // Serial.
  //
  // Serial is only necessary for debugging.

#if USE_SERIAL

    Serial.begin(115200);
    while(!Serial){};
    
#endif
  
  // Buttons (the buttons are located on the Adafruit afOled module).
  
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
  
  // Display.
  
    afOled.init();
    afOled.clearDisplay();

  // Wifi.

    // Set the wifi mode to access point and station mode.
    
    WiFi.mode(WIFI_AP_STA);

    // Set the station mode ssid and password.
    
    WiFi.begin(sSsidRouter.c_str(), sPassword.c_str());

    // Set the access point ip address, gateway and subnet.
    
    WiFi.softAPConfig(IpAddress, IpGateway, IpSubnet);

    // Set the access point ssid and password.
    
    WiFi.softAP(sSsidAccessPoint.c_str(), sPassword.c_str());

  // Server.
  
      // Root is the webpage.
      //
      // "/" is sent from the http client and is a request for a complete webpage refresh.
      
        wsWebServer.on("/", []()
        {
   wsWebServer.send(200, "text/html", LadyBuggyWebPage);
        });

      // Start the server.
      
        wsWebServer.begin();
  
      // Start the socket.
        
        webSocket.begin();
        webSocket.onEvent(LadyBuggySocketEvent);

      // Add http and ws services to MDNS.
      //
      // Notes:
      //
      //  1) "http" service is for the html portion of the web page and is serviced on port number PORT_HTTP.
      //  2) "ws" service is for the javascript socket portion of the web page and is serviced on port nomber PORT_WS.

        MDNS.begin("LadyBuggy", WiFi.localIP());

        MDNS.addService("http", "tcp", PORT_HTTP);
        MDNS.addService("ws", "tcp", PORT_WS);

  // Servos.
  
    ServoLeft.write(SERVO_OFF);
    ServoLeft.attach(12);

    ServoRight.write(SERVO_OFF);
    ServoRight.attach(13);

  //  Send the ip address to the serial monitor.

#if USE_SERIAL

    Serial.print("Lady Buggy: waiting for wifi connect");
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nLady Buggy: wifi connect with IP Address : " + WiFi.localIP().toString());
    
 #endif
} 
 
////////////////////////////////////////////////////////////////////
//
// Lady Buggy Main loop.
//
////////////////////////////////////////////////////////////////////////////

void loop() 
{ 
  // Update server.
  
    wsWebServer.handleClient();

  // Update socket.

    webSocket.loop();

  // Update oled display.
  
    LadyBuggyDisplay();
  
  // Update buttons.
  
    LadyBuggyButtons();
  
    // Give up some time.
    
    delay(20);
} 

//
// LadyBuggyButtons
// Update and process push buttons.
// Entry   : nothing
// Returns : nothing
//

void  LadyBuggyButtons()
{
  // Variables.
  
  static  int   nButtonAN1 = 1;
  static  int   nButtonBN1 = 1;
  static  int   nButtonCN1 = 1;
  
  // Read buttons.
  
  int nDigitalReadA = digitalRead(BUTTON_A);
  int nDigitalReadB = digitalRead(BUTTON_B);
  int nDigitalReadC = digitalRead(BUTTON_C);
  
  // Process button A.

  if(nDigitalReadA != nButtonAN1)
  {
    // State changed.
    
    if(nButtonAN1)
    {
      // Pressed.
      
      nButtonAN1 = 0;
      nButtonA = 1;
    }
    else
    {
      // Released.
      
      nButtonAN1 = 1;
      nButtonA = 0;
    }
  }

  // Process button B.
  
  if(nDigitalReadB != nButtonBN1)
  {
    // State changed.
    
    if(nButtonBN1)
    {
      // Pressed.
      
      nButtonBN1 = 0;
      nButtonB = 1;
    }
    else
    {
      // Released.
      
      nButtonBN1 = 1;
      nButtonB = 0;
    }
  }

  // Process button C.
  
  if(nDigitalReadC != nButtonCN1)
  {
    // State changed.
    
    if(nButtonCN1)
    {
      // Pressed.
      
      nButtonCN1 = 0;
      nButtonC = 1;
    }
    else
    {
      // Released.
      
      nButtonCN1 = 1;
      nButtonC = 0;
    }
  }
}

//
// LadyBuggyDisplay
// Update afOled display.
// Entry   : nothing
// Returns : nothing
//

//
// LadyBuggyDisplay
// Update the oled display
// Entry   : nothing
// Returns : nothing
//

void  LadyBuggyDisplay()
{  
  // Oled display.

  afOled.clearDisplay();

  String      sString = WiFi.localIP().toString();
  afOled.setTextSize(1);
  afOled.setCursor((DISPLAY_WIDTH / 2) - ((sString.length() * FONT_ONE_WIDTH) / 2), 0);
  afOled.print(sString);
  
  sString = String(String(nButtonA) + ", " + String(nButtonB) + ", " + String(nButtonC));
  afOled.setTextSize(1);
  afOled.setCursor((DISPLAY_WIDTH / 2) - ((sString.length() * FONT_ONE_WIDTH) / 2), (DISPLAY_HEIGHT / 2) - (FONT_ONE_HEIGHT / 2));
  afOled.print(sString);
 
  sString = String(String((100 * (nX - 50)) / 50) + ", " + String((100 * (50 - nY)) / 50));
  afOled.setTextSize(1);
  afOled.setCursor((DISPLAY_WIDTH / 2) - ((sString.length() * FONT_ONE_WIDTH) / 2), DISPLAY_HEIGHT - FONT_ONE_HEIGHT);
  afOled.print(sString);
 
  // Present the display.
  
  afOled.display();
}

////////////////////////////////////////////////////////////////
// LadyBuggySocketEvent
// Process socket client events.
// Entry   : client number
//  : event type
//  : pointer to message received from client
//  : length of message received from client
// Returns : nothing
//
/////////////////////////////////////////////////////////////////////////////

void LadyBuggySocketEvent(unsigned char ClientNumber, WStype_t EventType, unsigned char * Message, int MessageLength)
{
    // Process event type.
    
    switch(EventType)
    {
        // Process event type connect.
        
        case WStype_CONNECTED:
        {
     // Display connected client ip and url.
     
     IPAddress ip = webSocket.remoteIP(ClientNumber);

#if USE_SERIAL

     Serial.printf("Client #%u connect :\n", ClientNumber);
     Serial.printf("   ip address     : %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
//     Serial.printf("   url     : %s\n", Message);

#endif
     // Send a message to client
     
     webSocket.sendTXT(ClientNumber, "You are connected.");
        }
        break;

        // Process event type client text.
        
        case WStype_TEXT:
        {
     // Validate the incoming Lady Buggy data string by checking the first 5 characters for "zpllc".
     
     if((Message[0] == 'z') &&
        (Message[1] == 'p') &&
        (Message[2] == 'l') &&
        (Message[3] == 'l') &&
        (Message[4] == 'c'))
     {
         // Valid Lady Buggy header, convert incoming Lady Buggy data string to x and y.

      // Convert the string to double.
      
      double data = strtod((const char *) & Message[5], NULL);

      // Derive and limit the x component.
      
      nX = (int)data / 100;
      nX = (nX > 0) ? (nX < 99) ? nX : 99 : 0;

      // Derive and limit the y component.
      
      nY = (int)data % 100;
      nY = (nY > 0) ? (nY < 99) ? nY : 99 : 0;

      // Set the servo values based on the x and y components.
      
      int   nServoLeftScale = (((nX < 50) ? nX : 50) * 100) / 50;
      int   nServoRightScale = (((nX > 50) ? (100 - nX) : 50) * 100) / 50;
           
      ServoLeft.write(SERVO_OFF + ((nServoLeftScale * (nY - 50)) / 100));
      ServoRight.write(SERVO_OFF - ((nServoRightScale * (nY - 50)) / 100));

      // Send x and y components to the serial monitor.
      
#if USE_SERIAL

      Serial.printf("Client #%u sent: x = %02d, y = %02d\n", ClientNumber, nX, nY);
      
#endif

     }
     else
     {
         // Invalid Lady Buggy header, send data to serial monitor.
         
#if USE_SERIAL 
        
         Serial.printf("Client #%u invalid data: %s\n", ClientNumber, Message);
         
#endif

     }
        }
        break;

        // Process event type disconnect.
        
        case WStype_DISCONNECTED:
        {
     // Process client disconnect.
     
#if USE_SERIAL    
 
     Serial.printf("Client #%u disconnected.\n", ClientNumber);
     
#endif

        }
        break;
    }
}


