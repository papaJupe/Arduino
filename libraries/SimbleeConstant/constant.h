
  // possible modes
 const int MODE_UNKNOWN = 0;
 const int MODE_PASSIVE = 1;
 const int MODE_SAFE    = 2;
 const int MODE_FULL    = 3;

  // Roomba ROI opcodes -- these should all be bytes
 const int START   =  128;  // 0  # of param bytes needed
 const int BAUD    =  129;  // 1
 const int CONTROL =  130;  // 0
 const int SAFE    =  131;  // 0
 const int FULL    =  132;  // 0
 const int POWER   =  133;  // 0
 const int SPOT    =  134;  // 0
 const int CLEAN   =  135;  // 0
 const int MAX     =  136;  // 0
 const int DRIVE   =  137;  // 4
 const int MOTORS  =  138;  // 1
 const int LEDS    =  139;  // 3
 const int SONG    =  140;  // 2N+2
 const int PLAY    =  141;  // 1
 const int SENSORS =  142;  // 1
 const int DOCK    =  143;  // 0
 const int PWMMOTORS = 144; // 3
 const int DRIVEWHEELS = 145;  // 4
 const int DRIVEPWM = 146;      // 4
 const int STREAM  =  148;       // N+1
 const int QUERYLIST = 149;       // N+1
 const int STOPSTARTSTREAM = 150;  // 1
 const int SCHEDULINGLEDS = 162;   // 2
 const int DIGITLEDSRAW = 163;    // 4
 const int DIGITLEDSASCII = 164;  // 4
 const int BUTTONSCMD  =  165;   // 1
 const int SCHEDULE =  167;     // n
 const int SETDAYTIME = 168;   // 3

  // offsets (indices) of sensor_bytes data
 const int BUMPSWHEELDROPS     = 0;
 const int WALL                = 1;
 const int CLIFFLEFT           = 2;
 const int CLIFFFRONTLEFT      = 3;
 const int CLIFFFRONTRIGHT     = 4;
 const int CLIFFRIGHT          = 5;
 const int VIRTUALWALL         = 6;
 const int MOTOROVERCURRENTS   = 7;
 const int DIRTLEFT            = 8;
 const int DIRTRIGHT           = 9;
 const int REMOTEOPCODE        = 10;
 const int BUTTONS             = 11;
 const int DISTANCE_HI         = 12;
 const int DISTANCE_LO         = 13;  
 const int ANGLE_HI            = 14;
 const int ANGLE_LO            = 15;
 const int CHARGINGSTATE       = 16;
 const int VOLTAGE_HI          = 17;
 const int VOLTAGE_LO          = 18;  
 const int CURRENT_HI          = 19;
 const int CURRENT_LO          = 20;
 const int TEMPERATURE         = 21;
 const int CHARGE_HI           = 22;
 const int CHARGE_LO           = 23;
 const int CAPACITY_HI         = 24;
 const int CAPACITY_LO         = 25;

  // bitmasks for various things
 const int WHEELDROP_MASK      = 0x1C;
 const int BUMP_MASK           = 0x03;
 const int BUMPRIGHT_MASK      = 0x01;
 const int BUMPLEFT_MASK       = 0x02;

 const int WHEELDROPRIGHT_MASK = 0x04;
 const int WHEELDROPLEFT_MASK  = 0x08;
 const int WHEELDROPCENT_MASK  = 0x10;

 const int MOTORDRIVELEFT_MASK = 0x10;
 const int MOTORDRIVERIGHT_MASK= 0x08;
 const int MOTORMAINBRUSH_MASK = 0x04;
 const int MOTORVACUUM_MASK    = 0x02;
 const int MOTORSIDEBRUSH_MASK = 0x01;

 const int POWERBUTTON_MASK    = 0x08;  
 const int SPOTBUTTON_MASK     = 0x04;  
 const int CLEANBUTTON_MASK    = 0x02;  
 const int MAXBUTTON_MASK      = 0x01;  

  // which sensor packet to get, argument for sensors(int)
 const int SENSORS_ALL         = 0;
 const int SENSORS_PHYSICAL    = 1;
 const int SENSORS_INTERNAL    = 2;
 const int SENSORS_POWER       = 3;