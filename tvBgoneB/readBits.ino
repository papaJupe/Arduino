/* 
 This is kind of a strange but very useful helper function.
 Because we are using compression, we index to the CODES (timer) table
 not with a full 8-bit byte (which is wasteful) but 2 or 3 bits.
 Once code_ptr is set to point to the right Code bits,
 this function will let us read 'count' bits at a time which
 it does by reading a byte into 'bits_r' and then buffering it for return.
 */
 
// we can't read more than 8 bits at a time so don't try!
uint8_t read_bits(uint8_t count)
{
  // uint8_t bitsleft_r = 0;  // used in read_bits, val set by sendAllC, needs to be global?
  // uint8_t bits_r = 0;   // used only here, but as local doesn't work
  // uint8_t i;  // used in loop only, type it there
  uint8_t tmp = 0;

  // we need to read back count bytes
  for (uint8_t i=0; i<count; i++) 
  {
    // check if the 8-bit buffer has emptied
    if (bitsleft_r == 0) 
    {
      // in which case we read a new byte in from global ptr set in sendAllCodes
      bits_r = pgm_read_byte(code_ptr++);
      // and reset the buffer size (8 bits in a byte)
      bitsleft_r = 8;
    } //    end if == 0
      // remove one bit
    bitsleft_r--;
    // and shift it off of the end of 'bits_r'
    tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count-1-i));
  }  // end for loop w/ count; return bits to sendAllCodes, from LSB end of tmp
  return tmp;
}  // end read_bits()

