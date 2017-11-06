// Orig 128*64 OLED modified for 128*32 OLED: http://www.aliexpress.com/item/5pcs-0-91-inch-OLED-module-0-91-white-OLED-128X32-OLED-LCD-LED-Display-Module/32673572252.html
 
 /*
$GPRMC
http://aprs.gids.nl/nmea/#rmc

Recommended minimum specific GPS/Transit data

eg1. $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
eg2. $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68


           225446       Time of fix 22:54:46 UTC
           A            Navigation receiver warning A = OK, V = warning
           4916.45,N    Latitude 49 deg. 16.45 min North
           12311.12,W   Longitude 123 deg. 11.12 min West
           000.5        Speed over ground, Knots
           054.7        Course Made Good, True
           191194       Date of fix  19 November 1994
           020.3,E      Magnetic variation 20.3 deg East
           *68          mandatory checksum


eg3. $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
              1    2    3    4    5     6    7    8      9     10  11 12


      1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum


eg4. $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
1    = UTC of position fix
2    = Data status (V=navigation receiver warning)
3    = Latitude of fix
4    = N or S
5    = Longitude of fix
6    = E or W
7    = Speed over ground in knots
8    = Track made good in degrees True
9    = UT date
10   = Magnetic variation degrees (Easterly var. subtracts from true course)
11   = E or W
12   = Checksum



    =============================== OLED Connections =============================
    OLED: http://www.aliexpress.com/item/Free-shipping-1Pcs-white-128X64-OLED-LCD-LED-Display-Module-For-Arduino-0-96-I2C-IIC/32234039563.html
    Connections://Wire.pins(int sda, int scl)  Wire.pins(0, 2); //on ESP-01. 
    ==== ESP8266 Pin====    ==== OLED Pin====
            Vcc  1                Vcc  1
            Rxd  2                
            Rst  3                
           GPIO0 4                SDA  4             use 1.8K Ohm pullup
           CH_PD 5                
           CPIO2 6                SCL  3             " ditto "
             Txd 7                
             Gnd 8                Gnd  2

Credits:
 Modified by DanBicks added OLED display to ESP01 using cheap Ebay 0.96" display
 Lightweight Mike-Rankin OLED routines added in this project, Credits to Mike for this.
 http://www.esp8266.com/viewtopic.php?f=29&t=3256


This 128x32 OLED version departs from the previous OLED functins and utilizes an encapsulated
library: ./esp8266-oled-ssd1306/SSD1306.h
Such that a complete ZIP file can be created for both the primary sketch & library.
*/

