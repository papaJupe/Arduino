/*
TV-B-Gone for Arduino version 0.001
Ported to Arduino by Ken Shirriff, Dec 3, 2009
http://arcfn.com

The original code is:
TV-B-Gone Firmware version 1.2
 for use with ATtiny85v and v1.2 hardware
 (c) Mitch Altman + Limor Fried 2009
 mead mod: eliminated all but 6 EU codes, tried reordering, but then compile failed;
 reordering the pointer list seems to work, putting my LG earlier in sequence
*/

//Codes captured from Generation 3 TV-B-Gone by Limor Fried & Mitch Altman
//table of POWER codes -- same as pork fellow, 2013 rev., may not  have all device codes 2016

#include "main.h"

const uint16_t code_na000Times[] PROGMEM = {
  60, 60,
  60, 2700,
  120, 60,
  240, 60,
};
const uint8_t code_na000Codes[] PROGMEM = {
  0xE2,
  0x20,
  0x80,
  0x78,
  0x88,
  0x20,
  0x10,
};
const struct IrCode code_na000Code PROGMEM = {
  freq_to_timerval(38400),
  26,             // # of pairs
  2,              // # of bits per index
  code_na000Times,
  code_na000Codes
};

const uint16_t code_na001Times[] PROGMEM = {
  50, 100,
  50, 200,
  50, 800,
  400, 400,
};
const uint8_t code_na001Codes[] PROGMEM = {
  0xD5,
  0x41,
  0x11,
  0x00,
  0x14,
  0x44,
  0x6D,
  0x54,
  0x11,
  0x10,
  0x01,
  0x44,
  0x45,
};
const struct IrCode code_na001Code PROGMEM = {
  freq_to_timerval(57143),
  52,		// # of pairs
  2,		// # of bits per index
  code_na001Times,
  code_na001Codes
};
const uint16_t code_na002Times[] PROGMEM = {
  42, 46,
  42, 133,
  42, 7519,
  347, 176,
  347, 177,
};
const uint8_t code_na002Codes[] PROGMEM = {
  0x60,
  0x80,
  0x00,
  0x00,
  0x00,
  0x08,
  0x00,
  0x00,
  0x00,
  0x20,
  0x00,
  0x00,
  0x04,
  0x12,
  0x48,
  0x04,
  0x12,
  0x48,
  0x2A,
  0x02,
  0x00,
  0x00,
  0x00,
  0x00,
  0x20,
  0x00,
  0x00,
  0x00,
  0x80,
  0x00,
  0x00,
  0x10,
  0x49,
  0x20,
  0x10,
  0x49,
  0x20,
  0x80,
};
const struct IrCode code_na002Code PROGMEM = {
  freq_to_timerval(37037),
  100,		// # of pairs
  3,		// # of bits per index
  code_na002Times,
  code_na002Codes
};
const uint16_t code_na003Times[] PROGMEM = {
  26, 185,
  27, 80,
  27, 185,
  27, 4549,
};
const uint8_t code_na003Codes[] PROGMEM = {
  0x15,
  0x5A,
  0x65,
  0x67,
  0x95,
  0x65,
  0x9A,
  0x9B,
  0x95,
  0x5A,
  0x65,
  0x67,
  0x95,
  0x65,
  0x9A,
  0x99,
};
const struct IrCode code_na003Code PROGMEM = {
  freq_to_timerval(38610),
  64,		// # of pairs
  2,		// # of bits per index
  code_na003Times,
  code_na003Codes
};
const uint16_t code_na004Times[] PROGMEM = {
  55, 57,
  55, 170,
  55, 3949,
  55, 9623,
  56, 0,
  898, 453,
  900, 226,
};
const uint8_t code_na004Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x01,
  0x04,
  0x92,
  0x48,
  0x20,
  0x80,
  0x40,
  0x04,
  0x12,
  0x09,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na004Code PROGMEM = {
  freq_to_timerval(38610),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na004Codes
};
const uint16_t code_na005Times[] PROGMEM = {
  88, 90,
  88, 91,
  88, 181,
  88, 8976,
  177, 91,
};
const uint8_t code_na005Codes[] PROGMEM = {
  0x10,
  0x92,
  0x49,
  0x46,
  0x33,
  0x09,
  0x24,
  0x94,
  0x60,
};
const struct IrCode code_na005Code PROGMEM = {
  freq_to_timerval(35714),
  24,		// # of pairs
  3,		// # of bits per index
  code_na005Times,
  code_na005Codes
};
const uint16_t code_na006Times[] PROGMEM = {
  50, 62,
  50, 172,
  50, 4541,
  448, 466,
  450, 465,
};
const uint8_t code_na006Codes[] PROGMEM = {
  0x64,
  0x90,
  0x00,
  0x04,
  0x90,
  0x00,
  0x00,
  0x80,
  0x00,
  0x04,
  0x12,
  0x49,
  0x2A,
  0x12,
  0x40,
  0x00,
  0x12,
  0x40,
  0x00,
  0x02,
  0x00,
  0x00,
  0x10,
  0x49,
  0x24,
  0x90,
};
const struct IrCode code_na006Code PROGMEM = {
  freq_to_timerval(38462),
  68,		// # of pairs
  3,		// # of bits per index
  code_na006Times,
  code_na006Codes
};
const uint16_t code_na007Times[] PROGMEM = {
  49, 49,
  49, 50,
  49, 410,
  49, 510,
  49, 12107,
};
const uint8_t code_na007Codes[] PROGMEM = {
  0x09,
  0x94,
  0x53,
  0x29,
  0x94,
  0xD9,
  0x85,
  0x32,
  0x8A,
  0x65,
  0x32,
  0x9B,
  0x20,
};
const struct IrCode code_na007Code PROGMEM = {
  freq_to_timerval(39216),
  34,		// # of pairs
  3,		// # of bits per index
  code_na007Times,
  code_na007Codes
};
const uint16_t code_na008Times[] PROGMEM = {
  56, 58,
  56, 170,
  56, 4011,
  898, 450,
  900, 449,
};
const uint8_t code_na008Codes[] PROGMEM = {
  0x64,
  0x00,
  0x49,
  0x00,
  0x92,
  0x00,
  0x20,
  0x82,
  0x01,
  0x04,
  0x10,
  0x48,
  0x2A,
  0x10,
  0x01,
  0x24,
  0x02,
  0x48,
  0x00,
  0x82,
  0x08,
  0x04,
  0x10,
  0x41,
  0x20,
  0x90,
};
const struct IrCode code_na008Code PROGMEM = {
  freq_to_timerval(38462),
  68,		// # of pairs
  3,		// # of bits per index
  code_na008Times,
  code_na008Codes
};
const uint16_t code_na009Times[] PROGMEM = {
  53, 56,
  53, 171,
  53, 3950,
  53, 9599,
  898, 451,
  900, 226,
};
const uint8_t code_na009Codes[] PROGMEM = {
  0x84,
  0x90,
  0x00,
  0x20,
  0x80,
  0x08,
  0x00,
  0x00,
  0x09,
  0x24,
  0x92,
  0x40,
  0x0A,
  0xBA,
  0x40,
};
const struct IrCode code_na009Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na009Codes
};
const uint16_t code_na010Times[] PROGMEM = {
  51, 55,
  51, 158,
  51, 2286,
  841, 419,
};
const uint8_t code_na010Codes[] PROGMEM = {
  0xD4,
  0x00,
  0x15,
  0x10,
  0x25,
  0x00,
  0x05,
  0x44,
  0x09,
  0x40,
  0x01,
  0x51,
  0x01,
};
const struct IrCode code_na010Code PROGMEM = {
  freq_to_timerval(38462),
  52,		// # of pairs
  2,		// # of bits per index
  code_na010Times,
  code_na010Codes
};
const uint16_t code_na011Times[] PROGMEM = {
  55, 55,
  55, 172,
  55, 4039,
  55, 9348,
  56, 0,
  884, 442,
  885, 225,
};
const uint8_t code_na011Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x41,
  0x04,
  0x92,
  0x08,
  0x24,
  0x90,
  0x40,
  0x00,
  0x02,
  0x09,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na011Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na011Times,
  code_na011Codes
};
const uint16_t code_na012Times[] PROGMEM = {
  81, 87,
  81, 254,
  81, 3280,
  331, 336,
  331, 337,
};
const uint8_t code_na012Codes[] PROGMEM = {
  0x64,
  0x12,
  0x08,
  0x24,
  0x00,
  0x08,
  0x20,
  0x10,
  0x09,
  0x2A,
  0x10,
  0x48,
  0x20,
  0x90,
  0x00,
  0x20,
  0x80,
  0x40,
  0x24,
  0x90,
};
const struct IrCode code_na012Code PROGMEM = {
  freq_to_timerval(38462),
  52,		// # of pairs
  3,		// # of bits per index
  code_na012Times,
  code_na012Codes
};
const uint16_t code_na013Times[] PROGMEM = {
  53, 55,
  53, 167,
  53, 2304,
  53, 9369,
  893, 448,
  895, 447,
};
const uint8_t code_na013Codes[] PROGMEM = {
  0x80,
  0x12,
  0x40,
  0x04,
  0x00,
  0x09,
  0x00,
  0x12,
  0x41,
  0x24,
  0x82,
  0x01,
  0x00,
  0x10,
  0x48,
  0x24,
  0xAA,
  0xE8,
};
const struct IrCode code_na013Code PROGMEM = {
  freq_to_timerval(38462),
  48,		// # of pairs
  3,		// # of bits per index
  code_na013Times,
  code_na013Codes
};

/* Duplicate timing table, same as na004 !
 const uint16_t code_na014Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };
 */
const uint8_t code_na014Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x09,
  0x04,
  0x92,
  0x40,
  0x24,
  0x80,
  0x00,
  0x00,
  0x12,
  0x49,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na014Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na014Codes
};

/* Duplicate timing table, same as na004 !
 const uint16_t code_na015Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };
 */
const uint8_t code_na015Codes[] PROGMEM = {
  0xA0,
  0x80,
  0x01,
  0x04,
  0x12,
  0x48,
  0x24,
  0x00,
  0x00,
  0x00,
  0x92,
  0x49,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na015Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na015Codes
};
const uint16_t code_na016Times[] PROGMEM = {
  28, 90,
  28, 211,
  28, 2507,
};
const uint8_t code_na016Codes[] PROGMEM = {
  0x54,
  0x04,
  0x10,
  0x00,
  0x95,
  0x01,
  0x04,
  0x00,
  0x10,
};
const struct IrCode code_na016Code PROGMEM = {
  freq_to_timerval(34483),
  34,		// # of pairs
  2,		// # of bits per index
  code_na016Times,
  code_na016Codes
};
const uint16_t code_na017Times[] PROGMEM = {
  56, 57,
  56, 175,
  56, 4150,
  56, 9499,
  898, 227,
  898, 449,
};
const uint8_t code_na017Codes[] PROGMEM = {
  0xA0,
  0x02,
  0x48,
  0x04,
  0x90,
  0x01,
  0x20,
  0x80,
  0x40,
  0x04,
  0x12,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na017Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na017Codes
};
const uint16_t code_na018Times[] PROGMEM = {
  51, 55,
  51, 161,
  51, 2566,
  849, 429,
  849, 430,
};
const uint8_t code_na018Codes[] PROGMEM = {
  0x60,
  0x82,
  0x08,
  0x24,
  0x10,
  0x41,
  0x00,
  0x12,
  0x40,
  0x04,
  0x80,
  0x09,
  0x2A,
  0x02,
  0x08,
  0x20,
  0x90,
  0x41,
  0x04,
  0x00,
  0x49,
  0x00,
  0x12,
  0x00,
  0x24,
  0xA8,
  0x08,
  0x20,
  0x82,
  0x41,
  0x04,
  0x10,
  0x01,
  0x24,
  0x00,
  0x48,
  0x00,
  0x92,
  0xA0,
  0x20,
  0x82,
  0x09,
  0x04,
  0x10,
  0x40,
  0x04,
  0x90,
  0x01,
  0x20,
  0x02,
  0x48,
};
const struct IrCode code_na018Code PROGMEM = {
  freq_to_timerval(38462),
  136,		// # of pairs
  3,		// # of bits per index
  code_na018Times,
  code_na018Codes
};
const uint16_t code_na019Times[] PROGMEM = {
  40, 42,
  40, 124,
  40, 4601,
  325, 163,
  326, 163,
};
const uint8_t code_na019Codes[] PROGMEM = {
  0x60,
  0x10,
  0x40,
  0x04,
  0x80,
  0x09,
  0x00,
  0x00,
  0x00,
  0x00,
  0x10,
  0x00,
  0x20,
  0x10,
  0x00,
  0x20,
  0x80,
  0x00,
  0x0A,
  0x00,
  0x41,
  0x00,
  0x12,
  0x00,
  0x24,
  0x00,
  0x00,
  0x00,
  0x00,
  0x40,
  0x00,
  0x80,
  0x40,
  0x00,
  0x82,
  0x00,
  0x00,
  0x00,
};
const struct IrCode code_na019Code PROGMEM = {
  freq_to_timerval(38462),
  100,		// # of pairs
  3,		// # of bits per index
  code_na019Times,
  code_na019Codes
};


const uint16_t code_na020Times[] PROGMEM = {
  60, 55,
  60, 163,
  60, 4099,
  60, 9698,
  61, 0,
  898, 461,
  900, 230,
};
const uint8_t code_na020Codes[] PROGMEM = {
  0xA0,
  0x10,
  0x00,
  0x04,
  0x82,
  0x49,
  0x20,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na020Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na020Times,
  code_na020Codes
};
const uint16_t code_na021Times[] PROGMEM = {
  48, 52,
  48, 160,
  48, 400,
  48, 2335,
  799, 400,
};
const uint8_t code_na021Codes[] PROGMEM = {
  0x80,
  0x10,
  0x40,
  0x08,
  0x82,
  0x08,
  0x01,
  0xC0,
  0x08,
  0x20,
  0x04,
  0x41,
  0x04,
  0x00,
  0x00,
};
const struct IrCode code_na021Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na021Times,
  code_na021Codes
};
const uint16_t code_na022Times[] PROGMEM = {
  53, 60,
  53, 175,
  53, 4463,
  53, 9453,
  892, 450,
  895, 225,
};
const uint8_t code_na022Codes[] PROGMEM = {
  0x80,
  0x02,
  0x40,
  0x00,
  0x02,
  0x40,
  0x00,
  0x00,
  0x01,
  0x24,
  0x92,
  0x48,
  0x0A,
  0xBA,
  0x00,
};
const struct IrCode code_na022Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na022Times,
  code_na022Codes
};
const uint16_t code_na023Times[] PROGMEM = {
  48, 52,
  48, 409,
  48, 504,
  48, 10461,
};
const uint8_t code_na023Codes[] PROGMEM = {
  0xA1,
  0x18,
  0x61,
  0xA1,
  0x18,
  0x7A,
  0x11,
  0x86,
  0x1A,
  0x11,
  0x86,
};
const struct IrCode code_na023Code PROGMEM = {
  freq_to_timerval(40000),
  44,		// # of pairs
  2,		// # of bits per index
  code_na023Times,
  code_na023Codes
};
const uint16_t code_na024Times[] PROGMEM = {
  58, 60,
  58, 2569,
  118, 60,
  237, 60,
  238, 60,
};
const uint8_t code_na024Codes[] PROGMEM = {
  0x69,
  0x24,
  0x10,
  0x40,
  0x03,
  0x12,
  0x48,
  0x20,
  0x80,
  0x00,
};
const struct IrCode code_na024Code PROGMEM = {
  freq_to_timerval(38462),
  26,		// # of pairs
  3,		// # of bits per index
  code_na024Times,
  code_na024Codes
};
const uint16_t code_na025Times[] PROGMEM = {
  84, 90,
  84, 264,
  84, 3470,
  346, 350,
  347, 350,
};
const uint8_t code_na025Codes[] PROGMEM = {
  0x64,
  0x92,
  0x49,
  0x00,
  0x00,
  0x00,
  0x00,
  0x02,
  0x49,
  0x2A,
  0x12,
  0x49,
  0x24,
  0x00,
  0x00,
  0x00,
  0x00,
  0x09,
  0x24,
  0x90,
};
const struct IrCode code_na025Code PROGMEM = {
  freq_to_timerval(38462),
  52,		// # of pairs
  3,		// # of bits per index
  code_na025Times,
  code_na025Codes
};
const uint16_t code_na026Times[] PROGMEM = {
  49, 49,
  49, 50,
  49, 410,
  49, 510,
  49, 12582,
};
const uint8_t code_na026Codes[] PROGMEM = {
  0x09,
  0x94,
  0x53,
  0x65,
  0x32,
  0x99,
  0x85,
  0x32,
  0x8A,
  0x6C,
  0xA6,
  0x53,
  0x20,
};
const struct IrCode code_na026Code PROGMEM = {
  freq_to_timerval(39216),
  34,		// # of pairs
  3,		// # of bits per index
  code_na026Times,
  code_na026Codes
};

// Duplicate timing table, same as na001 !
 const uint16_t code_na027Times[] PROGMEM = {
 	50, 100,
 	50, 200,
 	50, 800,
 	400, 400,
 };
 
const uint8_t code_na027Codes[] PROGMEM = {
  0xC5,
  0x41,
  0x11,
  0x10,
  0x14,
  0x44,
  0x6C,
  0x54,
  0x11,
  0x11,
  0x01,
  0x44,
  0x44,
};
const struct IrCode code_na027Code PROGMEM = {
  freq_to_timerval(57143),
  52,		// # of pairs
  2,		// # of bits per index
  code_na001Times,
  code_na027Codes
};
const uint16_t code_na028Times[] PROGMEM = {
  118, 121,
  118, 271,
  118, 4750,
  258, 271,
};
const uint8_t code_na028Codes[] PROGMEM = {
  0xC4,
  0x45,
  0x14,
  0x04,
  0x6C,
  0x44,
  0x51,
  0x40,
  0x44,
};
const struct IrCode code_na028Code PROGMEM = {
  freq_to_timerval(38610),
  36,		// # of pairs
  2,		// # of bits per index
  code_na028Times,
  code_na028Codes
};
const uint16_t code_na029Times[] PROGMEM = {
  88, 90,
  88, 91,
  88, 181,
  177, 91,
  177, 8976,
};
const uint8_t code_na029Codes[] PROGMEM = {
  0x0C,
  0x92,
  0x53,
  0x46,
  0x16,
  0x49,
  0x29,
  0xA2,
  0xC0,
};
const struct IrCode code_na029Code PROGMEM = {
  freq_to_timerval(35842),
  22,		// # of pairs
  3,		// # of bits per index
  code_na029Times,
  code_na029Codes
};

// Duplicate timing table, same as na009 !
 const uint16_t code_na030Times[] PROGMEM = {
 	53, 56,
 	53, 171,
 	53, 3950,
 	53, 9599,
 	898, 451,
 	900, 226,
 };
 
const uint8_t code_na030Codes[] PROGMEM = {
  0x80,
  0x00,
  0x41,
  0x04,
  0x12,
  0x08,
  0x20,
  0x00,
  0x00,
  0x04,
  0x92,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na030Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na030Codes
};
const uint16_t code_na031Times[] PROGMEM = {
  88, 89,
  88, 90,
  88, 179,
  88, 8977,
  177, 90,
};
const uint8_t code_na031Codes[] PROGMEM = {
  0x06,
  0x12,
  0x49,
  0x46,
  0x32,
  0x61,
  0x24,
  0x94,
  0x60,
};
const struct IrCode code_na031Code PROGMEM = {
  freq_to_timerval(35842),
  24,		// # of pairs
  3,		// # of bits per index
  code_na031Times,
  code_na031Codes
};

// Duplicate timing table, same as na009 !
 const uint16_t code_na032Times[] PROGMEM = {
 	53, 56,
 	53, 171,
 	53, 3950,
 	53, 9599,
 	898, 451,
 	900, 226,
 };
 
const uint8_t code_na032Codes[] PROGMEM = {
  0x80,
  0x00,
  0x41,
  0x04,
  0x12,
  0x08,
  0x20,
  0x80,
  0x00,
  0x04,
  0x12,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na032Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na032Codes
};
const uint16_t code_na033Times[] PROGMEM = {
  40, 43,
  40, 122,
  40, 5297,
  334, 156,
  336, 155,
};
const uint8_t code_na033Codes[] PROGMEM = {
  0x60,
  0x10,
  0x40,
  0x04,
  0x80,
  0x09,
  0x00,
  0x00,
  0x00,
  0x00,
  0x10,
  0x00,
  0x20,
  0x82,
  0x00,
  0x20,
  0x00,
  0x00,
  0x0A,
  0x00,
  0x41,
  0x00,
  0x12,
  0x00,
  0x24,
  0x00,
  0x00,
  0x00,
  0x00,
  0x40,
  0x00,
  0x82,
  0x08,
  0x00,
  0x80,
  0x00,
  0x00,
  0x00,
};
const struct IrCode code_na033Code PROGMEM = {
  freq_to_timerval(38462),
  100,		// # of pairs
  3,		// # of bits per index
  code_na033Times,
  code_na033Codes
};

//  Duplicate timing table, same as na004 !
 const uint16_t code_na034Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };

const uint8_t code_na034Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x41,
  0x04,
  0x92,
  0x08,
  0x24,
  0x92,
  0x48,
  0x00,
  0x00,
  0x01,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na034Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na034Codes
};
const uint16_t code_na035Times[] PROGMEM = {
  96, 93,
  97, 93,
  97, 287,
  97, 3431,
};
const uint8_t code_na035Codes[] PROGMEM = {
  0x16,
  0x66,
  0x5D,
  0x59,
  0x99,
  0x50,
};
const struct IrCode code_na035Code PROGMEM = {
  freq_to_timerval(41667),
  22,		// # of pairs
  2,		// # of bits per index
  code_na035Times,
  code_na035Codes
};
const uint16_t code_na036Times[] PROGMEM = {
  82, 581,
  84, 250,
  84, 580,
  85, 0,
};
const uint8_t code_na036Codes[] PROGMEM = {
  0x15,
  0x9A,
  0x9C,
};
const struct IrCode code_na036Code PROGMEM = {
  freq_to_timerval(37037),
  11,		// # of pairs
  2,		// # of bits per index
  code_na036Times,
  code_na036Codes
};
const uint16_t code_na037Times[] PROGMEM = {
  39, 263,
  164, 163,
  514, 164,
};
const uint8_t code_na037Codes[] PROGMEM = {
  0x80,
  0x45,
  0x00,
};
const struct IrCode code_na037Code PROGMEM = {
  freq_to_timerval(41667),
  11,		// # of pairs
  2,		// # of bits per index
  code_na037Times,
  code_na037Codes
};

// Duplicate timing table, same as na017 !
 const uint16_t code_na038Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 
const uint8_t code_na038Codes[] PROGMEM = {
  0xA4,
  0x10,
  0x40,
  0x00,
  0x82,
  0x09,
  0x20,
  0x80,
  0x40,
  0x04,
  0x12,
  0x09,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na038Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na038Codes
};
const uint16_t code_na039Times[] PROGMEM = {
  113, 101,
  688, 2707,
};
const uint8_t code_na039Codes[] PROGMEM = {
  0x11,
};
const struct IrCode code_na039Code PROGMEM = {
  freq_to_timerval(40000),
  4,		// # of pairs
  2,		// # of bits per index
  code_na039Times,
  code_na039Codes
};

const uint16_t code_na040Times[] PROGMEM = {
  113, 101,
  113, 201,
  113, 2707,
};
const uint8_t code_na040Codes[] PROGMEM = {
  0x06,
  0x04,
};
const struct IrCode code_na040Code PROGMEM = {
  freq_to_timerval(40000),
  8,		// # of pairs
  2,		// # of bits per index
  code_na040Times,
  code_na040Codes
};
const uint16_t code_na041Times[] PROGMEM = {
  58, 62,
  58, 2746,
  117, 62,
  242, 62,
};
const uint8_t code_na041Codes[] PROGMEM = {
  0xE2,
  0x20,
  0x80,
  0x78,
  0x88,
  0x20,
  0x00,
};
const struct IrCode code_na041Code PROGMEM = {
  freq_to_timerval(76923),
  26,		// # of pairs
  2,		// # of bits per index
  code_na041Times,
  code_na041Codes
};
const uint16_t code_na042Times[] PROGMEM = {
  54, 65,
  54, 170,
  54, 4099,
  54, 8668,
  899, 226,
  899, 421,
};
const uint8_t code_na042Codes[] PROGMEM = {
  0xA4,
  0x80,
  0x00,
  0x20,
  0x82,
  0x49,
  0x00,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na042Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na042Times,
  code_na042Codes
};
const uint16_t code_na043Times[] PROGMEM = {
  43, 120,
  43, 121,
  43, 3491,
  131, 45,
};
const uint8_t code_na043Codes[] PROGMEM = {
  0x15,
  0x75,
  0x56,
  0x55,
  0x75,
  0x54,
};
const struct IrCode code_na043Code PROGMEM = {
  freq_to_timerval(40000),
  24,		// # of pairs
  2,		// # of bits per index
  code_na043Times,
  code_na043Codes
};
const uint16_t code_na044Times[] PROGMEM = {
  51, 51,
  51, 160,
  51, 4096,
  51, 9513,
  431, 436,
  883, 219,
};
const uint8_t code_na044Codes[] PROGMEM = {
  0x84,
  0x90,
  0x00,
  0x00,
  0x02,
  0x49,
  0x20,
  0x80,
  0x00,
  0x04,
  0x12,
  0x49,
  0x2A,
  0xBA,
  0x40,
};
const struct IrCode code_na044Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na044Times,
  code_na044Codes
};
const uint16_t code_na045Times[] PROGMEM = {
  58, 53,
  58, 167,
  58, 4494,
  58, 9679,
  455, 449,
  456, 449,
};
const uint8_t code_na045Codes[] PROGMEM = {
  0x80,
  0x90,
  0x00,
  0x00,
  0x90,
  0x00,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0x97,
  0x48,
};
const struct IrCode code_na045Code PROGMEM = {
  freq_to_timerval(38462),
  40,		// # of pairs
  3,		// # of bits per index
  code_na045Times,
  code_na045Codes
};
const uint16_t code_na046Times[] PROGMEM = {
  51, 277,
  52, 53,
  52, 105,
  52, 277,
  52, 2527,
  52, 12809,
  103, 54,
};
const uint8_t code_na046Codes[] PROGMEM = {
  0x0B,
  0x12,
  0x63,
  0x44,
  0x92,
  0x6B,
  0x44,
  0x92,
  0x50,
};
const struct IrCode code_na046Code PROGMEM = {
  freq_to_timerval(29412),
  23,		// # of pairs
  3,		// # of bits per index
  code_na046Times,
  code_na046Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na047Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na047Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x40,
  0x04,
  0x92,
  0x09,
  0x24,
  0x92,
  0x09,
  0x20,
  0x00,
  0x40,
  0x0A,
  0x38,
  0x00,
};
const struct IrCode code_na047Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na047Codes
};

/* Duplicate timing table, same as na044 !
 const uint16_t code_na048Times[] PROGMEM = {
 	51, 51,
 	51, 160,
 	51, 4096,
 	51, 9513,
 	431, 436,
 	883, 219,
 };
 */
const uint8_t code_na048Codes[] PROGMEM = {
  0x80,
  0x00,
  0x00,
  0x04,
  0x92,
  0x49,
  0x24,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na048Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na044Times,
  code_na048Codes
};
const uint16_t code_na049Times[] PROGMEM = {
  274, 854,
  274, 1986,
};
const uint8_t code_na049Codes[] PROGMEM = {
  0x14,
  0x11,
  0x40,
};
const struct IrCode code_na049Code PROGMEM = {
  freq_to_timerval(45455),
  11,		// # of pairs
  2,		// # of bits per index
  code_na049Times,
  code_na049Codes
};
const uint16_t code_na050Times[] PROGMEM = {
  80, 88,
  80, 254,
  80, 3750,
  359, 331,
};
const uint8_t code_na050Codes[] PROGMEM = {
  0xC0,
  0x00,
  0x01,
  0x55,
  0x55,
  0x52,
  0xC0,
  0x00,
  0x01,
  0x55,
  0x55,
  0x50,
};
const struct IrCode code_na050Code PROGMEM = {
  freq_to_timerval(55556),
  48,		// # of pairs
  2,		// # of bits per index
  code_na050Times,
  code_na050Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na051Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na051Codes[] PROGMEM = {
  0xA0,
  0x10,
  0x01,
  0x24,
  0x82,
  0x48,
  0x00,
  0x02,
  0x40,
  0x04,
  0x90,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na051Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na051Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na052Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na052Codes[] PROGMEM = {
  0xA4,
  0x90,
  0x48,
  0x00,
  0x02,
  0x01,
  0x20,
  0x80,
  0x40,
  0x04,
  0x12,
  0x09,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na052Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na052Codes
};
const uint16_t code_na053Times[] PROGMEM = {
  51, 232,
  51, 512,
  51, 792,
  51, 2883,
};
const uint8_t code_na053Codes[] PROGMEM = {
  0x22,
  0x21,
  0x40,
  0x1C,
  0x88,
  0x85,
  0x00,
  0x40,
};
const struct IrCode code_na053Code PROGMEM = {
  freq_to_timerval(55556),
  30,		// # of pairs
  2,		// # of bits per index
  code_na053Times,
  code_na053Codes
};

/* Duplicate timing table, same as na053 !
 const uint16_t code_na054Times[] PROGMEM = {
 	51, 232,
 	51, 512,
 	51, 792,
 	51, 2883,
 };
 */
const uint8_t code_na054Codes[] PROGMEM = {
  0x22,
  0x20,
  0x15,
  0x72,
  0x22,
  0x01,
  0x54,
};
const struct IrCode code_na054Code PROGMEM = {
  freq_to_timerval(55556),
  28,		// # of pairs
  2,		// # of bits per index
  code_na053Times,
  code_na054Codes
};
const uint16_t code_na055Times[] PROGMEM = {
  3, 10,
  3, 20,
  3, 30,
  3, 12778,
};
const uint8_t code_na055Codes[] PROGMEM = {
  0x81,
  0x51,
  0x14,
  0xB8,
  0x15,
  0x11,
  0x44,
};
const struct IrCode code_na055Code PROGMEM = {
  0,              // Non-pulsed code
  27,		// # of pairs
  2,		// # of bits per index
  code_na055Times,
  code_na055Codes
};
const uint16_t code_na056Times[] PROGMEM = {
  55, 193,
  57, 192,
  57, 384,
  58, 0,
};
const uint8_t code_na056Codes[] PROGMEM = {
  0x2A,
  0x57,
};
const struct IrCode code_na056Code PROGMEM = {
  freq_to_timerval(37175),
  8,		// # of pairs
  2,		// # of bits per index
  code_na056Times,
  code_na056Codes
};
const uint16_t code_na057Times[] PROGMEM = {
  45, 148,
  46, 148,
  46, 351,
  46, 2781,
};
const uint8_t code_na057Codes[] PROGMEM = {
  0x2A,
  0x5D,
  0xA9,
  0x60,
};
const struct IrCode code_na057Code PROGMEM = {
  freq_to_timerval(40000),
  14,		// # of pairs
  2,		// # of bits per index
  code_na057Times,
  code_na057Codes
};
const uint16_t code_na058Times[] PROGMEM = {
  22, 101,
  22, 219,
  23, 101,
  23, 219,
  31, 218,
};
const uint8_t code_na058Codes[] PROGMEM = {
  0x8D,
  0xA4,
  0x08,
  0x04,
  0x04,
  0x92,
  0x4C,
};
const struct IrCode code_na058Code PROGMEM = {
  freq_to_timerval(33333),
  18,		// # of pairs
  3,		// # of bits per index
  code_na058Times,
  code_na058Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na059Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na059Codes[] PROGMEM = {
  0xA4,
  0x12,
  0x09,
  0x00,
  0x80,
  0x40,
  0x20,
  0x10,
  0x40,
  0x04,
  0x82,
  0x09,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na059Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na059Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na060Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na060Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x08,
  0x04,
  0x92,
  0x41,
  0x24,
  0x00,
  0x40,
  0x00,
  0x92,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na060Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na060Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na061Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na061Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x08,
  0x24,
  0x92,
  0x41,
  0x04,
  0x82,
  0x00,
  0x00,
  0x10,
  0x49,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na061Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na061Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na062Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na062Codes[] PROGMEM = {
  0xA0,
  0x02,
  0x08,
  0x04,
  0x90,
  0x41,
  0x24,
  0x82,
  0x00,
  0x00,
  0x10,
  0x49,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na062Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na062Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na063Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na063Codes[] PROGMEM = {
  0xA4,
  0x92,
  0x49,
  0x20,
  0x00,
  0x00,
  0x04,
  0x92,
  0x48,
  0x00,
  0x00,
  0x01,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na063Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na063Codes
};

/* Duplicate timing table, same as na001 !
 const uint16_t code_na064Times[] PROGMEM = {
 	50, 100,
 	50, 200,
 	50, 800,
 	400, 400,
 };
 */
const uint8_t code_na064Codes[] PROGMEM = {
  0xC0,
  0x01,
  0x51,
  0x55,
  0x54,
  0x04,
  0x2C,
  0x00,
  0x15,
  0x15,
  0x55,
  0x40,
  0x40,
};
const struct IrCode code_na064Code PROGMEM = {
  freq_to_timerval(57143),
  52,		// # of pairs
  2,		// # of bits per index
  code_na001Times,
  code_na064Codes
};
const uint16_t code_na065Times[] PROGMEM = {
  48, 98,
  48, 197,
  98, 846,
  395, 392,
  1953, 392,
};
const uint8_t code_na065Codes[] PROGMEM = {
  0x84,
  0x92,
  0x01,
  0x24,
  0x12,
  0x00,
  0x04,
  0x80,
  0x08,
  0x09,
  0x92,
  0x48,
  0x04,
  0x90,
  0x48,
  0x00,
  0x12,
  0x00,
  0x20,
  0x26,
  0x49,
  0x20,
  0x12,
  0x41,
  0x20,
  0x00,
  0x48,
  0x00,
  0x80,
  0x80,
};
const struct IrCode code_na065Code PROGMEM = {
  freq_to_timerval(59172),
  78,		// # of pairs
  3,		// # of bits per index
  code_na065Times,
  code_na065Codes
};
const uint16_t code_na066Times[] PROGMEM = {
  38, 276,
  165, 154,
  415, 155,
  742, 154,
};
const uint8_t code_na066Codes[] PROGMEM = {
  0xC0,
  0x45,
  0x02,
  0x01,
  0x14,
  0x08,
  0x04,
  0x50,
  0x00,
};
const struct IrCode code_na066Code PROGMEM = {
  freq_to_timerval(38462),
  33,		// # of pairs
  2,		// # of bits per index
  code_na066Times,
  code_na066Codes
};

/* Duplicate timing table, same as na044 !
 const uint16_t code_na067Times[] PROGMEM = {
 	51, 51,
 	51, 160,
 	51, 4096,
 	51, 9513,
 	431, 436,
 	883, 219,
 };
 */
const uint8_t code_na067Codes[] PROGMEM = {
  0x80,
  0x02,
  0x49,
  0x24,
  0x90,
  0x00,
  0x00,
  0x80,
  0x00,
  0x04,
  0x12,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na067Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na044Times,
  code_na067Codes
};
const uint16_t code_na068Times[] PROGMEM = {
  43, 121,
  43, 9437,
  130, 45,
  131, 45,
};
const uint8_t code_na068Codes[] PROGMEM = {
  0x8C,
  0x30,
  0x0D,
  0xCC,
  0x30,
  0x0C,
};
const struct IrCode code_na068Code PROGMEM = {
  freq_to_timerval(40000),
  24,		// # of pairs
  2,		// # of bits per index
  code_na068Times,
  code_na068Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na069Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na069Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x00,
  0x04,
  0x92,
  0x49,
  0x24,
  0x82,
  0x00,
  0x00,
  0x10,
  0x49,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na069Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na069Codes
};
const uint16_t code_na070Times[] PROGMEM = {
  27, 76,
  27, 182,
  27, 183,
  27, 3199,
};
const uint8_t code_na070Codes[] PROGMEM = {
  0x40,
  0x02,
  0x08,
  0xA2,
  0xE0,
  0x00,
  0x82,
  0x28,
  0x40,
};
const struct IrCode code_na070Code PROGMEM = {
  freq_to_timerval(38462),
  33,		// # of pairs
  2,		// # of bits per index
  code_na070Times,
  code_na070Codes
};
const uint16_t code_na071Times[] PROGMEM = {
  37, 181,
  37, 272,
};
const uint8_t code_na071Codes[] PROGMEM = {
  0x11,
  0x40,
};
const struct IrCode code_na071Code PROGMEM = {
  freq_to_timerval(55556),
  8,		// # of pairs
  2,		// # of bits per index
  code_na071Times,
  code_na071Codes
};

/* Duplicate timing table, same as na042 !
 const uint16_t code_na072Times[] PROGMEM = {
 	54, 65,
 	54, 170,
 	54, 4099,
 	54, 8668,
 	899, 226,
 	899, 421,
 };
 */
const uint8_t code_na072Codes[] PROGMEM = {
  0xA0,
  0x90,
  0x00,
  0x00,
  0x90,
  0x00,
  0x00,
  0x10,
  0x40,
  0x04,
  0x82,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na072Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na042Times,
  code_na072Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na073Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na073Codes[] PROGMEM = {
  0xA0,
  0x82,
  0x08,
  0x24,
  0x10,
  0x41,
  0x00,
  0x00,
  0x00,
  0x24,
  0x92,
  0x49,
  0x0A,
  0x38,
  0x00,
};
const struct IrCode code_na073Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na073Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na074Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na074Codes[] PROGMEM = {
  0xA4,
  0x00,
  0x41,
  0x00,
  0x92,
  0x08,
  0x20,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na074Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na074Codes
};
const uint16_t code_na075Times[] PROGMEM = {
  51, 98,
  51, 194,
  102, 931,
  390, 390,
  390, 391,
};
const uint8_t code_na075Codes[] PROGMEM = {
  0x60,
  0x00,
  0x01,
  0x04,
  0x10,
  0x49,
  0x24,
  0x82,
  0x08,
  0x2A,
  0x00,
  0x00,
  0x04,
  0x10,
  0x41,
  0x24,
  0x92,
  0x08,
  0x20,
  0xA0,
};
const struct IrCode code_na075Code PROGMEM = {
  freq_to_timerval(41667),
  52,		// # of pairs
  3,		// # of bits per index
  code_na075Times,
  code_na075Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na076Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na076Codes[] PROGMEM = {
  0xA0,
  0x92,
  0x09,
  0x04,
  0x00,
  0x40,
  0x20,
  0x10,
  0x40,
  0x04,
  0x82,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na076Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na076Codes
};

/* Duplicate timing table, same as na031 !
 const uint16_t code_na077Times[] PROGMEM = {
 	88, 89,
 	88, 90,
 	88, 179,
 	88, 8977,
 	177, 90,
 };
 */
const uint8_t code_na077Codes[] PROGMEM = {
  0x10,
  0xA2,
  0x62,
  0x31,
  0x98,
  0x51,
  0x31,
  0x18,
  0x00,
};
const struct IrCode code_na077Code PROGMEM = {
  freq_to_timerval(35714),
  22,		// # of pairs
  3,		// # of bits per index
  code_na031Times,
  code_na077Codes
};
const uint16_t code_na078Times[] PROGMEM = {
  40, 275,
  160, 154,
  480, 155,
};
const uint8_t code_na078Codes[] PROGMEM = {
  0x80,
  0x45,
  0x04,
  0x01,
  0x14,
  0x10,
  0x04,
  0x50,
  0x40,
};
const struct IrCode code_na078Code PROGMEM = {
  freq_to_timerval(38462),
  34,		// # of pairs
  2,		// # of bits per index
  code_na078Times,
  code_na078Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na079Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na079Codes[] PROGMEM = {
  0xA0,
  0x82,
  0x08,
  0x24,
  0x10,
  0x41,
  0x04,
  0x90,
  0x08,
  0x20,
  0x02,
  0x41,
  0x0A,
  0x38,
  0x00,
};
const struct IrCode code_na079Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na079Codes
};

/* Duplicate timing table, same as na055 !
 const uint16_t code_na080Times[] PROGMEM = {
 	3, 10,
 	3, 20,
 	3, 30,
 	3, 12778,
 };
 */
const uint8_t code_na080Codes[] PROGMEM = {
  0x81,
  0x50,
  0x40,
  0xB8,
  0x15,
  0x04,
  0x08,
};
const struct IrCode code_na080Code PROGMEM = {
  0,              // Non-pulsed code
  27,		// # of pairs
  2,		// # of bits per index
  code_na055Times,
  code_na080Codes
};
const uint16_t code_na081Times[] PROGMEM = {
  48, 52,
  48, 409,
  48, 504,
  48, 9978,
};
const uint8_t code_na081Codes[] PROGMEM = {
  0x18,
  0x46,
  0x18,
  0x68,
  0x47,
  0x18,
  0x46,
  0x18,
  0x68,
  0x44,
};
const struct IrCode code_na081Code PROGMEM = {
  freq_to_timerval(40000),
  40,		// # of pairs
  2,		// # of bits per index
  code_na081Times,
  code_na081Codes
};
const uint16_t code_na082Times[] PROGMEM = {
  88, 89,
  88, 90,
  88, 179,
  88, 8888,
  177, 90,
  177, 179,
};
const uint8_t code_na082Codes[] PROGMEM = {
  0x0A,
  0x12,
  0x49,
  0x2A,
  0xB2,
  0xA1,
  0x24,
  0x92,
  0xA8,
};
const struct IrCode code_na082Code PROGMEM = {
  freq_to_timerval(35714),
  24,		// # of pairs
  3,		// # of bits per index
  code_na082Times,
  code_na082Codes
};

/* Duplicate timing table, same as na031 !
 const uint16_t code_na083Times[] PROGMEM = {
 	88, 89,
 	88, 90,
 	88, 179,
 	88, 8977,
 	177, 90,
 };
 */
const uint8_t code_na083Codes[] PROGMEM = {
  0x10,
  0x92,
  0x49,
  0x46,
  0x33,
  0x09,
  0x24,
  0x94,
  0x60,
};
const struct IrCode code_na083Code PROGMEM = {
  freq_to_timerval(35714),
  24,		// # of pairs
  3,		// # of bits per index
  code_na031Times,
  code_na083Codes
};

const uint16_t code_na084Times[] PROGMEM = {
  41, 43,
  41, 128,
  41, 7476,
  336, 171,
  338, 169,
};
const uint8_t code_na084Codes[] PROGMEM = {
  0x60,
  0x80,
  0x00,
  0x00,
  0x00,
  0x08,
  0x00,
  0x00,
  0x40,
  0x20,
  0x00,
  0x00,
  0x04,
  0x12,
  0x48,
  0x04,
  0x12,
  0x08,
  0x2A,
  0x02,
  0x00,
  0x00,
  0x00,
  0x00,
  0x20,
  0x00,
  0x01,
  0x00,
  0x80,
  0x00,
  0x00,
  0x10,
  0x49,
  0x20,
  0x10,
  0x48,
  0x20,
  0x80,
};
const struct IrCode code_na084Code PROGMEM = {
  freq_to_timerval(37037),
  100,		// # of pairs
  3,		// # of bits per index
  code_na084Times,
  code_na084Codes
};
const uint16_t code_na085Times[] PROGMEM = {
  55, 60,
  55, 165,
  55, 2284,
  445, 437,
  448, 436,
};
const uint8_t code_na085Codes[] PROGMEM = {
  0x64,
  0x00,
  0x00,
  0x00,
  0x00,
  0x40,
  0x00,
  0x80,
  0xA1,
  0x00,
  0x00,
  0x00,
  0x00,
  0x10,
  0x00,
  0x20,
  0x10,
};
const struct IrCode code_na085Code PROGMEM = {
  freq_to_timerval(38462),
  44,		// # of pairs
  3,		// # of bits per index
  code_na085Times,
  code_na085Codes
};
const uint16_t code_na086Times[] PROGMEM = {
  42, 46,
  42, 126,
  42, 6989,
  347, 176,
  347, 177,
};
const uint8_t code_na086Codes[] PROGMEM = {
  0x60,
  0x82,
  0x08,
  0x20,
  0x82,
  0x41,
  0x04,
  0x92,
  0x00,
  0x20,
  0x80,
  0x40,
  0x00,
  0x90,
  0x40,
  0x04,
  0x00,
  0x41,
  0x2A,
  0x02,
  0x08,
  0x20,
  0x82,
  0x09,
  0x04,
  0x12,
  0x48,
  0x00,
  0x82,
  0x01,
  0x00,
  0x02,
  0x41,
  0x00,
  0x10,
  0x01,
  0x04,
  0x80,
};
const struct IrCode code_na086Code PROGMEM = {
  freq_to_timerval(37175),
  100,		// # of pairs
  3,		// # of bits per index
  code_na086Times,
  code_na086Codes
};
const uint16_t code_na087Times[] PROGMEM = {
  56, 69,
  56, 174,
  56, 4165,
  56, 9585,
  880, 222,
  880, 435,
};
const uint8_t code_na087Codes[] PROGMEM = {
  0xA0,
  0x02,
  0x40,
  0x04,
  0x90,
  0x09,
  0x20,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na087Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na087Times,
  code_na087Codes
};

/* Duplicate timing table, same as na009 !
 const uint16_t code_na088Times[] PROGMEM = {
 	53, 56,
 	53, 171,
 	53, 3950,
 	53, 9599,
 	898, 451,
 	900, 226,
 };
 */
const uint8_t code_na088Codes[] PROGMEM = {
  0x80,
  0x00,
  0x40,
  0x04,
  0x12,
  0x08,
  0x04,
  0x92,
  0x40,
  0x00,
  0x00,
  0x09,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na088Code PROGMEM = {
  freq_to_timerval(38610),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na088Codes
};

/* Duplicate timing table, same as na004 !
 const uint16_t code_na089Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };
 */
const uint8_t code_na089Codes[] PROGMEM = {
  0xA0,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x20,
  0x80,
  0x40,
  0x04,
  0x12,
  0x09,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na089Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na089Codes
};
const uint16_t code_na090Times[] PROGMEM = {
  88, 90,
  88, 91,
  88, 181,
  88, 8976,
  177, 91,
  177, 181,
};
const uint8_t code_na090Codes[] PROGMEM = {
  0x10,
  0xAB,
  0x11,
  0x8C,
  0xC2,
  0xAC,
  0x46,
  0x00,
};
const struct IrCode code_na090Code PROGMEM = {
  freq_to_timerval(35714),
  20,		// # of pairs
  3,		// # of bits per index
  code_na090Times,
  code_na090Codes
};
const uint16_t code_na091Times[] PROGMEM = {
  48, 100,
  48, 200,
  48, 1050,
  400, 400,
};
const uint8_t code_na091Codes[] PROGMEM = {
  0xD5,
  0x41,
  0x51,
  0x40,
  0x14,
  0x04,
  0x2D,
  0x54,
  0x15,
  0x14,
  0x01,
  0x40,
  0x41,
};
const struct IrCode code_na091Code PROGMEM = {
  freq_to_timerval(58824),
  52,		// # of pairs
  2,		// # of bits per index
  code_na091Times,
  code_na091Codes
};
const uint16_t code_na092Times[] PROGMEM = {
  54, 56,
  54, 170,
  54, 4927,
  451, 447,
};
const uint8_t code_na092Codes[] PROGMEM = {
  0xD1,
  0x00,
  0x11,
  0x00,
  0x04,
  0x00,
  0x11,
  0x55,
  0x6D,
  0x10,
  0x01,
  0x10,
  0x00,
  0x40,
  0x01,
  0x15,
  0x55,
};
const struct IrCode code_na092Code PROGMEM = {
  freq_to_timerval(38462),
  68,		// # of pairs
  2,		// # of bits per index
  code_na092Times,
  code_na092Codes
};
const uint16_t code_na093Times[] PROGMEM = {
  55, 57,
  55, 167,
  55, 4400,
  895, 448,
  897, 447,
};
const uint8_t code_na093Codes[] PROGMEM = {
  0x60,
  0x90,
  0x00,
  0x20,
  0x80,
  0x00,
  0x04,
  0x02,
  0x01,
  0x00,
  0x90,
  0x48,
  0x2A,
  0x02,
  0x40,
  0x00,
  0x82,
  0x00,
  0x00,
  0x10,
  0x08,
  0x04,
  0x02,
  0x41,
  0x20,
  0x80,
};
const struct IrCode code_na093Code PROGMEM = {
  freq_to_timerval(38462),
  68,		// # of pairs
  3,		// # of bits per index
  code_na093Times,
  code_na093Codes
};

/* Duplicate timing table, same as na005 !
 const uint16_t code_na094Times[] PROGMEM = {
 	88, 90,
 	88, 91,
 	88, 181,
 	88, 8976,
 	177, 91,
 };
 */
const uint8_t code_na094Codes[] PROGMEM = {
  0x10,
  0x94,
  0x62,
  0x31,
  0x98,
  0x4A,
  0x31,
  0x18,
  0x00,
};
const struct IrCode code_na094Code PROGMEM = {
  freq_to_timerval(35714),
  22,		// # of pairs
  3,		// # of bits per index
  code_na005Times,
  code_na094Codes
};
const uint16_t code_na095Times[] PROGMEM = {
  56, 58,
  56, 174,
  56, 4549,
  56, 9448,
  440, 446,
};
const uint8_t code_na095Codes[] PROGMEM = {
  0x80,
  0x02,
  0x00,
  0x00,
  0x02,
  0x00,
  0x04,
  0x82,
  0x00,
  0x00,
  0x10,
  0x49,
  0x2A,
  0x17,
  0x08,
};
const struct IrCode code_na095Code PROGMEM = {
  freq_to_timerval(38462),
  40,		// # of pairs
  3,		// # of bits per index
  code_na095Times,
  code_na095Codes
};

/* Duplicate timing table, same as na009 !
 const uint16_t code_na096Times[] PROGMEM = {
 	53, 56,
 	53, 171,
 	53, 3950,
 	53, 9599,
 	898, 451,
 	900, 226,
 };
 */
const uint8_t code_na096Codes[] PROGMEM = {
  0x80,
  0x80,
  0x40,
  0x04,
  0x92,
  0x49,
  0x20,
  0x92,
  0x00,
  0x04,
  0x00,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na096Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na096Codes
};

/* Duplicate timing table, same as na009 !
 const uint16_t code_na097Times[] PROGMEM = {
 	53, 56,
 	53, 171,
 	53, 3950,
 	53, 9599,
 	898, 451,
 	900, 226,
 };
 */
const uint8_t code_na097Codes[] PROGMEM = {
  0x84,
  0x80,
  0x00,
  0x24,
  0x10,
  0x41,
  0x00,
  0x80,
  0x01,
  0x24,
  0x12,
  0x48,
  0x0A,
  0xBA,
  0x40,
};
const struct IrCode code_na097Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na097Codes
};

/* Duplicate timing table, same as na004 !
 const uint16_t code_na098Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };
 */
const uint8_t code_na098Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x00,
  0x04,
  0x92,
  0x49,
  0x24,
  0x00,
  0x41,
  0x00,
  0x92,
  0x08,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na098Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na098Codes
};

/* Duplicate timing table, same as na009 !
 const uint16_t code_na099Times[] PROGMEM = {
 	53, 56,
 	53, 171,
 	53, 3950,
 	53, 9599,
 	898, 451,
 	900, 226,
 };
 */
const uint8_t code_na099Codes[] PROGMEM = {
  0x80,
  0x00,
  0x00,
  0x04,
  0x12,
  0x48,
  0x24,
  0x00,
  0x00,
  0x00,
  0x92,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na099Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na009Times,
  code_na099Codes
};
const uint16_t code_na100Times[] PROGMEM = {
  43, 171,
  45, 60,
  45, 170,
  54, 2301,
};
const uint8_t code_na100Codes[] PROGMEM = {
  0x29,
  0x59,
  0x65,
  0x55,
  0xEA,
  0x56,
  0x59,
  0x55,
  0x70,
};
const struct IrCode code_na100Code PROGMEM = {
  freq_to_timerval(35842),
  34,		// # of pairs
  2,		// # of bits per index
  code_na100Times,
  code_na100Codes
};

/* Duplicate timing table, same as na004 !
 const uint16_t code_na101Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };
 */
const uint8_t code_na101Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x09,
  0x04,
  0x92,
  0x40,
  0x20,
  0x00,
  0x00,
  0x04,
  0x92,
  0x49,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na101Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na101Codes
};
const uint16_t code_na102Times[] PROGMEM = {
  86, 87,
  86, 258,
  86, 3338,
  346, 348,
  348, 347,
};
const uint8_t code_na102Codes[] PROGMEM = {
  0x64,
  0x02,
  0x08,
  0x00,
  0x02,
  0x09,
  0x04,
  0x12,
  0x49,
  0x0A,
  0x10,
  0x08,
  0x20,
  0x00,
  0x08,
  0x24,
  0x10,
  0x49,
  0x24,
  0x10,
};
const struct IrCode code_na102Code PROGMEM = {
  freq_to_timerval(40000),
  52,		// # of pairs
  3,		// # of bits per index
  code_na102Times,
  code_na102Codes
};

/* Duplicate timing table, same as na045 !
 const uint16_t code_na103Times[] PROGMEM = {
 	58, 53,
 	58, 167,
 	58, 4494,
 	58, 9679,
 	455, 449,
 	456, 449,
 };
 */
const uint8_t code_na103Codes[] PROGMEM = {
  0x80,
  0x02,
  0x00,
  0x00,
  0x02,
  0x00,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0x97,
  0x48,
};
const struct IrCode code_na103Code PROGMEM = {
  freq_to_timerval(38462),
  40,		// # of pairs
  3,		// # of bits per index
  code_na045Times,
  code_na103Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na104Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na104Codes[] PROGMEM = {
  0xA4,
  0x00,
  0x49,
  0x00,
  0x92,
  0x00,
  0x20,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na104Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na104Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na105Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na105Codes[] PROGMEM = {
  0xA4,
  0x80,
  0x00,
  0x20,
  0x12,
  0x49,
  0x04,
  0x92,
  0x49,
  0x20,
  0x00,
  0x00,
  0x0A,
  0x38,
  0x40,
};
const struct IrCode code_na105Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na105Codes
};

/* Duplicate timing table, same as na044 !
 const uint16_t code_na106Times[] PROGMEM = {
 	51, 51,
 	51, 160,
 	51, 4096,
 	51, 9513,
 	431, 436,
 	883, 219,
 };
 */
const uint8_t code_na106Codes[] PROGMEM = {
  0x80,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x24,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na106Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na044Times,
  code_na106Codes
};

/* Duplicate timing table, same as na045 !
 const uint16_t code_na107Times[] PROGMEM = {
 	58, 53,
 	58, 167,
 	58, 4494,
 	58, 9679,
 	455, 449,
 	456, 449,
 };
 */
const uint8_t code_na107Codes[] PROGMEM = {
  0x80,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0x97,
  0x48,
};
const struct IrCode code_na107Code PROGMEM = {
  freq_to_timerval(38462),
  40,		// # of pairs
  3,		// # of bits per index
  code_na045Times,
  code_na107Codes
};

/* Duplicate timing table, same as na045 !
 const uint16_t code_na108Times[] PROGMEM = {
 	58, 53,
 	58, 167,
 	58, 4494,
 	58, 9679,
 	455, 449,
 	456, 449,
 };
 */
const uint8_t code_na108Codes[] PROGMEM = {
  0x80,
  0x90,
  0x40,
  0x00,
  0x90,
  0x40,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0x97,
  0x48,
};
const struct IrCode code_na108Code PROGMEM = {
  freq_to_timerval(38462),
  40,		// # of pairs
  3,		// # of bits per index
  code_na045Times,
  code_na108Codes
};
const uint16_t code_na109Times[] PROGMEM = {
  58, 61,
  58, 211,
  58, 9582,
  73, 4164,
  883, 211,
  1050, 494,
};
const uint8_t code_na109Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x08,
  0x24,
  0x92,
  0x41,
  0x00,
  0x82,
  0x00,
  0x04,
  0x10,
  0x49,
  0x2E,
  0x28,
  0x00,
};
const struct IrCode code_na109Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na109Times,
  code_na109Codes
};


/* Duplicate timing table, same as na017 !
 const uint16_t code_na110Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na110Codes[] PROGMEM = {
  0xA4,
  0x80,
  0x00,
  0x20,
  0x12,
  0x49,
  0x00,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na110Code PROGMEM = {
  freq_to_timerval(40161),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na110Codes
};

/* Duplicate timing table, same as na044 !
 const uint16_t code_na111Times[] PROGMEM = {
 	51, 51,
 	51, 160,
 	51, 4096,
 	51, 9513,
 	431, 436,
 	883, 219,
 };
 */
const uint8_t code_na111Codes[] PROGMEM = {
  0x84,
  0x92,
  0x49,
  0x20,
  0x00,
  0x00,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0xBA,
  0x40,
};
const struct IrCode code_na111Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na044Times,
  code_na111Codes
};

/* Duplicate timing table, same as na004 !
 const uint16_t code_na112Times[] PROGMEM = {
 	55, 57,
 	55, 170,
 	55, 3949,
 	55, 9623,
 	56, 0,
 	898, 453,
 	900, 226,
 };
 */
const uint8_t code_na112Codes[] PROGMEM = {
  0xA4,
  0x00,
  0x00,
  0x00,
  0x92,
  0x49,
  0x24,
  0x00,
  0x00,
  0x00,
  0x92,
  0x49,
  0x2B,
  0x3D,
  0x00,
};
const struct IrCode code_na112Code PROGMEM = {
  freq_to_timerval(38462),
  38,		// # of pairs
  3,		// # of bits per index
  code_na004Times,
  code_na112Codes
};
const uint16_t code_na113Times[] PROGMEM = {
  56, 54,
  56, 166,
  56, 3945,
  896, 442,
  896, 443,
};
const uint8_t code_na113Codes[] PROGMEM = {
  0x60,
  0x00,
  0x00,
  0x20,
  0x02,
  0x09,
  0x04,
  0x02,
  0x01,
  0x00,
  0x90,
  0x48,
  0x2A,
  0x00,
  0x00,
  0x00,
  0x80,
  0x08,
  0x24,
  0x10,
  0x08,
  0x04,
  0x02,
  0x41,
  0x20,
  0x80,
};
const struct IrCode code_na113Code PROGMEM = {
  freq_to_timerval(40000),
  68,		// # of pairs
  3,		// # of bits per index
  code_na113Times,
  code_na113Codes
};
const uint16_t code_na114Times[] PROGMEM = {
  44, 50,
  44, 147,
  44, 447,
  44, 2236,
  791, 398,
  793, 397,
};
const uint8_t code_na114Codes[] PROGMEM = {
  0x84,
  0x10,
  0x40,
  0x08,
  0x82,
  0x08,
  0x01,
  0xD2,
  0x08,
  0x20,
  0x04,
  0x41,
  0x04,
  0x00,
  0x40,
};
const struct IrCode code_na114Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na114Times,
  code_na114Codes
};


const uint16_t code_na115Times[] PROGMEM = {
  81, 86,
  81, 296,
  81, 3349,
  328, 331,
  329, 331,
};
const uint8_t code_na115Codes[] PROGMEM = {
  0x60,
  0x82,
  0x00,
  0x20,
  0x80,
  0x41,
  0x04,
  0x90,
  0x41,
  0x2A,
  0x02,
  0x08,
  0x00,
  0x82,
  0x01,
  0x04,
  0x12,
  0x41,
  0x04,
  0x80,
};
const struct IrCode code_na115Code PROGMEM = {
  freq_to_timerval(40000),
  52,		// # of pairs
  3,		// # of bits per index
  code_na115Times,
  code_na115Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na116Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na116Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x40,
  0x04,
  0x92,
  0x09,
  0x24,
  0x00,
  0x40,
  0x00,
  0x92,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na116Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na116Codes
};
const uint16_t code_na117Times[] PROGMEM = {
  49, 54,
  49, 158,
  49, 420,
  49, 2446,
  819, 420,
  821, 419,
};
const uint8_t code_na117Codes[] PROGMEM = {
  0x84,
  0x00,
  0x00,
  0x08,
  0x12,
  0x40,
  0x01,
  0xD2,
  0x00,
  0x00,
  0x04,
  0x09,
  0x20,
  0x00,
  0x40,
};
const struct IrCode code_na117Code PROGMEM = {
  freq_to_timerval(41667),
  38,		// # of pairs
  3,		// # of bits per index
  code_na117Times,
  code_na117Codes
};

/* Duplicate timing table, same as na044 !
 const uint16_t code_na118Times[] PROGMEM = {
 	51, 51,
 	51, 160,
 	51, 4096,
 	51, 9513,
 	431, 436,
 	883, 219,
 };
 */
const uint8_t code_na118Codes[] PROGMEM = {
  0x84,
  0x90,
  0x49,
  0x20,
  0x02,
  0x00,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0xBA,
  0x40,
};
const struct IrCode code_na118Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na044Times,
  code_na118Codes
};
const uint16_t code_na119Times[] PROGMEM = {
  55, 63,
  55, 171,
  55, 4094,
  55, 9508,
  881, 219,
  881, 438,
};
const uint8_t code_na119Codes[] PROGMEM = {
  0xA0,
  0x10,
  0x00,
  0x04,
  0x82,
  0x49,
  0x20,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na119Code PROGMEM = {
  freq_to_timerval(55556),
  38,		// # of pairs
  3,		// # of bits per index
  code_na119Times,
  code_na119Codes
};


/* Duplicate timing table, same as na017 !
 const uint16_t code_na120Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na120Codes[] PROGMEM = {
  0xA0,
  0x12,
  0x00,
  0x04,
  0x80,
  0x49,
  0x24,
  0x92,
  0x40,
  0x00,
  0x00,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na120Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na120Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na121Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na121Codes[] PROGMEM = {
  0xA0,
  0x00,
  0x40,
  0x04,
  0x92,
  0x09,
  0x20,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na121Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na121Codes
};
const uint16_t code_na122Times[] PROGMEM = {
  80, 95,
  80, 249,
  80, 3867,
  81, 0,
  329, 322,
};
const uint8_t code_na122Codes[] PROGMEM = {
  0x80,
  0x00,
  0x00,
  0x00,
  0x12,
  0x49,
  0x24,
  0x90,
  0x0A,
  0x80,
  0x00,
  0x00,
  0x00,
  0x12,
  0x49,
  0x24,
  0x90,
  0x0B,
};
const struct IrCode code_na122Code PROGMEM = {
  freq_to_timerval(52632),
  48,		// # of pairs
  3,		// # of bits per index
  code_na122Times,
  code_na122Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na123Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na123Codes[] PROGMEM = {
  0xA0,
  0x02,
  0x48,
  0x04,
  0x90,
  0x01,
  0x20,
  0x12,
  0x40,
  0x04,
  0x80,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na123Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na123Codes
};
const uint16_t code_na124Times[] PROGMEM = {
  54, 56,
  54, 151,
  54, 4092,
  54, 8677,
  900, 421,
  901, 226,
};
const uint8_t code_na124Codes[] PROGMEM = {
  0x80,
  0x00,
  0x48,
  0x04,
  0x92,
  0x01,
  0x20,
  0x00,
  0x00,
  0x04,
  0x92,
  0x49,
  0x2A,
  0xBA,
  0x00,
};
const struct IrCode code_na124Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na124Times,
  code_na124Codes
};

/* Duplicate timing table, same as na119 !
 const uint16_t code_na125Times[] PROGMEM = {
 	55, 63,
 	55, 171,
 	55, 4094,
 	55, 9508,
 	881, 219,
 	881, 438,
 };
 */
const uint8_t code_na125Codes[] PROGMEM = {
  0xA0,
  0x02,
  0x48,
  0x04,
  0x90,
  0x01,
  0x20,
  0x80,
  0x40,
  0x04,
  0x12,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na125Code PROGMEM = {
  freq_to_timerval(55556),
  38,		// # of pairs
  3,		// # of bits per index
  code_na119Times,
  code_na125Codes
};


/* Duplicate timing table, same as na017 !
 const uint16_t code_na126Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na126Codes[] PROGMEM = {
  0xA4,
  0x10,
  0x00,
  0x20,
  0x82,
  0x49,
  0x00,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na126Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na126Codes
};
const uint16_t code_na127Times[] PROGMEM = {
  114, 100,
  115, 100,
  115, 200,
  115, 2706,
};
const uint8_t code_na127Codes[] PROGMEM = {
  0x1B,
  0x59,
};
const struct IrCode code_na127Code PROGMEM = {
  freq_to_timerval(25641),
  8,		// # of pairs
  2,		// # of bits per index
  code_na127Times,
  code_na127Codes
};

/* Duplicate timing table, same as na102 !
 const uint16_t code_na128Times[] PROGMEM = {
 	86, 87,
 	86, 258,
 	86, 3338,
 	346, 348,
 	348, 347,
 };
 */
const uint8_t code_na128Codes[] PROGMEM = {
  0x60,
  0x02,
  0x08,
  0x00,
  0x02,
  0x49,
  0x04,
  0x12,
  0x49,
  0x0A,
  0x00,
  0x08,
  0x20,
  0x00,
  0x09,
  0x24,
  0x10,
  0x49,
  0x24,
  0x00,
};
const struct IrCode code_na128Code PROGMEM = {
  freq_to_timerval(40000),
  52,		// # of pairs
  3,		// # of bits per index
  code_na102Times,
  code_na128Codes
};

/* Duplicate timing table, same as na017 !
 const uint16_t code_na129Times[] PROGMEM = {
 	56, 57,
 	56, 175,
 	56, 4150,
 	56, 9499,
 	898, 227,
 	898, 449,
 };
 */
const uint8_t code_na129Codes[] PROGMEM = {
  0xA4,
  0x92,
  0x49,
  0x20,
  0x00,
  0x00,
  0x00,
  0x02,
  0x00,
  0x04,
  0x90,
  0x49,
  0x2A,
  0x38,
  0x40,
};
const struct IrCode code_na129Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na017Times,
  code_na129Codes
};
const uint16_t code_na130Times[] PROGMEM = {
  88, 90,
  88, 258,
  88, 2247,
  358, 349,
  358, 350,
};
const uint8_t code_na130Codes[] PROGMEM = {
  0x64,
  0x00,
  0x08,
  0x24,
  0x82,
  0x09,
  0x24,
  0x10,
  0x01,
  0x0A,
  0x10,
  0x00,
  0x20,
  0x92,
  0x08,
  0x24,
  0x90,
  0x40,
  0x04,
  0x10,
};
const struct IrCode code_na130Code PROGMEM = {
  freq_to_timerval(37037),
  52,		// # of pairs
  3,		// # of bits per index
  code_na130Times,
  code_na130Codes
};

/* Duplicate timing table, same as na042 !
 const uint16_t code_na131Times[] PROGMEM = {
 	54, 65,
 	54, 170,
 	54, 4099,
 	54, 8668,
 	899, 226,
 	899, 421,
 };
 */
const uint8_t code_na131Codes[] PROGMEM = {
  0xA0,
  0x10,
  0x40,
  0x04,
  0x82,
  0x09,
  0x24,
  0x82,
  0x40,
  0x00,
  0x10,
  0x09,
  0x2A,
  0x38,
  0x00,
};
const struct IrCode code_na131Code PROGMEM = {
  freq_to_timerval(40000),
  38,		// # of pairs
  3,		// # of bits per index
  code_na042Times,
  code_na131Codes
};
const uint16_t code_na132Times[] PROGMEM = {
  28, 106,
  28, 238,
  28, 370,
  28, 1173,
};
const uint8_t code_na132Codes[] PROGMEM = {
  0x22,
  0x20,
  0x00,
  0x17,
  0x22,
  0x20,
  0x00,
  0x14,
};
const struct IrCode code_na132Code PROGMEM = {
  freq_to_timerval(83333),
  32,		// # of pairs
  2,		// # of bits per index
  code_na132Times,
  code_na132Codes
};
const uint16_t code_na133Times[] PROGMEM = {
  13, 741,
  15, 489,
  15, 740,
  17, 4641,
  18, 0,
};
const uint8_t code_na133Codes[] PROGMEM = {
  0x09,
  0x24,
  0x49,
  0x48,
  0xB4,
  0x92,
  0x44,
  0x94,
  0x8C,
};
const struct IrCode code_na133Code PROGMEM = {
  freq_to_timerval(41667),
  24,		// # of pairs
  3,		// # of bits per index
  code_na133Times,
  code_na133Codes
};

/* Duplicate timing table, same as na113 !
 const uint16_t code_na134Times[] PROGMEM = {
 	56, 54,
 	56, 166,
 	56, 3945,
 	896, 442,
 	896, 443,
 };
 */
const uint8_t code_na134Codes[] PROGMEM = {
  0x60,
  0x90,
  0x00,
  0x24,
  0x10,
  0x00,
  0x04,
  0x92,
  0x00,
  0x00,
  0x00,
  0x49,
  0x2A,
  0x02,
  0x40,
  0x00,
  0x90,
  0x40,
  0x00,
  0x12,
  0x48,
  0x00,
  0x00,
  0x01,
  0x24,
  0x80,
};
const struct IrCode code_na134Code PROGMEM = {
  freq_to_timerval(40000),
  68,		// # of pairs
  3,		// # of bits per index
  code_na113Times,
  code_na134Codes
};
const uint16_t code_na135Times[] PROGMEM = {
  53, 59,
  53, 171,
  53, 2301,
  892, 450,
  895, 448,
};
const uint8_t code_na135Codes[] PROGMEM = {
  0x60,
  0x12,
  0x49,
  0x00,
  0x00,
  0x09,
  0x00,
  0x00,
  0x49,
  0x24,
  0x80,
  0x00,
  0x00,
  0x12,
  0x49,
  0x24,
  0xA8,
  0x01,
  0x24,
  0x90,
  0x00,
  0x00,
  0x90,
  0x00,
  0x04,
  0x92,
  0x48,
  0x00,
  0x00,
  0x01,
  0x24,
  0x92,
  0x48,
};
const struct IrCode code_na135Code PROGMEM = {
  freq_to_timerval(38462),
  88,		// # of pairs
  3,		// # of bits per index
  code_na135Times,
  code_na135Codes
};
const uint16_t code_na136Times[] PROGMEM = {
  53, 59,
  53, 171,
  53, 2301,
  55, 0,
  892, 450,
  895, 448,
};
const uint8_t code_na136Codes[] PROGMEM = {
  0x84,
  0x82,
  0x49,
  0x00,
  0x00,
  0x00,
  0x20,
  0x00,
  0x49,
  0x24,
  0x80,
  0x00,
  0x00,
  0x12,
  0x49,
  0x24,
  0xAA,
  0x48,
  0x24,
  0x90,
  0x00,
  0x00,
  0x02,
  0x00,
  0x04,
  0x92,
  0x48,
  0x00,
  0x00,
  0x01,
  0x24,
  0x92,
  0x4B,
};
const struct IrCode code_na136Code PROGMEM = {
  freq_to_timerval(38610),
  88,		// # of pairs
  3,		// # of bits per index
  code_na136Times,
  code_na136Codes
};


// start EU table

const uint16_t code_eu000Times[] PROGMEM = {
  43, 47,
  43, 91,
  43, 8324,
  88, 47,
  133, 133,
  264, 90,
  264, 91,
};
const uint8_t code_eu000Codes[] PROGMEM = {
  0xA4,
  0x08,
  0x00,
  0x00,
  0x00,
  0x00,
  0x64,
  0x2C,
  0x40,
  0x80,
  0x00,
  0x00,
  0x00,
  0x06,
  0x41,
};
const struct IrCode code_eu000Code PROGMEM = {
  freq_to_timerval(35714),
  40,		// # of pairs
  3,		// # of bits per index
  code_eu000Times,
  code_eu000Codes
};
const uint16_t code_eu001Times[] PROGMEM = {
  47, 265,
  51, 54,
  51, 108,
  51, 263,
  51, 2053,
  51, 11647,
  100, 109,
};
const uint8_t code_eu001Codes[] PROGMEM = {
  0x04,
  0x92,
  0x49,
  0x26,
  0x35,
  0x89,
  0x24,
  0x9A,
  0xD6,
  0x24,
  0x92,
  0x48,
};
const struct IrCode code_eu001Code PROGMEM = {
  freq_to_timerval(30303),
  31,		// # of pairs
  3,		// # of bits per index
  code_eu001Times,
  code_eu001Codes
};
const uint16_t code_eu002Times[] PROGMEM = {
  43, 206,
  46, 204,
  46, 456,
  46, 3488,
};
const uint8_t code_eu002Codes[] PROGMEM = {
  0x1A,
  0x56,
  0xA6,
  0xD6,
  0x95,
  0xA9,
  0x90,
};
const struct IrCode code_eu002Code PROGMEM = {
  freq_to_timerval(33333),
  26,		// # of pairs
  2,		// # of bits per index
  code_eu002Times,
  code_eu002Codes
};

/* Duplicate timing table, same as na000 !
 const uint16_t code_eu003Times[] PROGMEM = {
 	58, 60,
 	58, 2687,
 	118, 60,
 	237, 60,
 	238, 60,
 };
 */
/*
const uint8_t code_eu003Codes[] PROGMEM = {
 	0x68,
 	0x20,
 	0x80,
 	0x40,
 	0x03,
 	0x10,
 	0x41,
 	0x00,
 	0x80,
 	0x00,
 };
 const struct IrCode code_eu003Code PROGMEM = {
 	freq_to_timerval(38462),
 	26,		// # of pairs
 	3,		// # of bits per index
 	code_na000Times,
 	code_eu003Codes
 };// Duplicate IR Code - same as na000
 */

const uint16_t code_eu004Times[] PROGMEM = {
  44, 45,
  44, 131,
  44, 7462,
  346, 176,
  346, 178,
};
const uint8_t code_eu004Codes[] PROGMEM = {
  0x60,
  0x80,
  0x00,
  0x00,
  0x00,
  0x08,
  0x00,
  0x00,
  0x00,
  0x20,
  0x00,
  0x00,
  0x04,
  0x12,
  0x48,
  0x04,
  0x12,
  0x48,
  0x2A,
  0x02,
  0x00,
  0x00,
  0x00,
  0x00,
  0x20,
  0x00,
  0x00,
  0x00,
  0x80,
  0x00,
  0x00,
  0x10,
  0x49,
  0x20,
  0x10,
  0x49,
  0x20,
  0x80,
};
const struct IrCode code_eu004Code PROGMEM = {
  freq_to_timerval(37037),
  100,		// # of pairs
  3,		// # of bits per index
  code_eu004Times,
  code_eu004Codes
};// Duplicate IR Code? Similar to NA002

const uint16_t code_eu005Times[] PROGMEM = {
  24, 190,
  25, 80,
  25, 190,
  25, 4199,
  25, 4799,
};
const uint8_t code_eu005Codes[] PROGMEM = {
  0x04,
  0x92,
  0x52,
  0x28,
  0x92,
  0x8C,
  0x44,
  0x92,
  0x89,
  0x45,
  0x24,
  0x53,
  0x44,
  0x92,
  0x52,
  0x28,
  0x92,
  0x8C,
  0x44,
  0x92,
  0x89,
  0x45,
  0x24,
  0x51,
};
const struct IrCode code_eu005Code PROGMEM = {
  freq_to_timerval(38610),
  64,		// # of pairs
  3,		// # of bits per index
  code_eu005Times,
  code_eu005Codes
};
////////////////////////////////////////////////////////////////

//const PROGMEM array (called "NApowerCodes") of const pointers to IrCode structs
//-otherwise stated: "declare NApowerCodes as PROGMEM array of const pointers to const IrCode structs"
//-to confirm this, go to http://cdecl.org/ and paste "const int* const NApowerCodes[]", and you'll 
// see it means "declare NApowerCodes as array of const pointer to const int"
const IrCode* const NApowerCodes[] PROGMEM = {
  &code_na020Code,
  &code_na021Code,
  &code_na022Code,
  &code_na023Code,
  &code_na024Code,
  &code_na025Code,
  &code_na026Code,
  &code_na027Code,
  &code_na028Code,
  &code_na029Code,
  &code_na030Code,
  &code_na031Code,
  &code_na032Code,
  &code_na033Code,
  &code_na034Code,
  &code_na035Code,
  &code_na036Code,
  &code_na037Code,
  &code_na038Code,
  &code_na039Code,
  &code_na040Code,
  &code_na041Code,
  &code_na042Code,
  &code_na043Code,
  &code_na044Code,
  &code_na045Code,
  &code_na046Code,
  &code_na047Code,
  &code_na048Code,
  &code_na049Code,
  &code_na000Code,
  &code_na001Code,
  &code_na002Code,
  &code_na003Code,
  &code_na004Code,
  &code_na005Code,
  &code_na006Code,
  &code_na007Code,
  &code_na008Code,
  &code_na009Code,
  &code_na010Code,
  &code_na011Code,
  &code_na012Code,
  &code_na013Code,
  &code_na014Code,
  &code_na015Code,
  &code_na016Code,
  &code_na017Code,
  &code_na018Code,
  &code_na019Code,
  &code_na050Code,
  &code_na051Code,
  &code_na052Code,
  &code_na053Code,
  &code_na054Code,
  &code_na055Code,
  &code_na056Code,
  &code_na057Code,
  &code_na058Code,
  &code_na059Code,
  &code_na060Code,
  &code_na061Code,
  &code_na062Code,
  &code_na063Code,
  &code_na064Code,
  &code_na065Code,
  &code_na066Code,
  &code_na067Code,
  &code_na068Code,
  &code_na069Code,
  &code_na070Code,
  &code_na071Code,
  &code_na072Code,
  &code_na073Code,
  &code_na074Code,
  &code_na075Code,
  &code_na076Code,
  &code_na077Code,
  &code_na078Code,
  &code_na079Code,
  &code_na080Code,
  &code_na081Code,
  &code_na082Code,
  &code_na083Code,
  &code_na084Code,
  &code_na085Code,
  &code_na086Code,
  &code_na087Code,
  &code_na088Code,
  &code_na089Code,
  &code_na090Code,
  &code_na091Code,
  &code_na092Code,
  &code_na093Code,
  &code_na094Code,
  &code_na095Code,
  &code_na096Code,
  &code_na097Code,
  &code_na098Code,
  &code_na099Code,
  &code_na100Code,
  &code_na101Code,
  &code_na102Code,
  &code_na103Code,
  &code_na104Code,
  &code_na105Code,
  &code_na106Code,
  &code_na107Code,
  &code_na108Code,
  &code_na109Code,
  &code_na110Code,
  &code_na111Code,
  &code_na112Code,
  &code_na113Code,
  &code_na114Code,
  &code_na115Code,
  &code_na116Code,
  &code_na117Code,
  &code_na118Code,
  &code_na119Code,
  &code_na120Code,
  &code_na121Code,
  &code_na122Code,
  &code_na123Code,
  &code_na124Code,
  &code_na125Code,
  &code_na126Code,
  &code_na127Code,
  &code_na128Code,
  &code_na129Code,
  &code_na130Code,
  &code_na131Code,
  &code_na132Code,
  &code_na133Code,
  &code_na134Code,
  &code_na135Code,
  &code_na136Code,
};

const IrCode* const EUpowerCodes[] PROGMEM = {
  &code_eu000Code,
  &code_eu001Code,
  &code_eu002Code,
  &code_na000Code, // same as &code_eu003Code
  &code_eu004Code,
  &code_eu005Code,
};

uint8_t num_NAcodes = NUM_ELEM(NApowerCodes);
uint8_t num_EUcodes = NUM_ELEM(EUpowerCodes);
