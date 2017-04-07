/*
progmemPointerExerc -- from Gammons post http://forum.arduino.cc/index.php?topic=302174.0
which had a few errors, but with correction, clarifies using PROGMEM and pointers to hold
lots of string data if too much for ardu's SRAM

Many tries here to use F() to print array strings from progmem directly failed; wants a string literal

Uses: char pointer, ptr to ptr, PGM_P, C++ class constr, strcpy_P,
*/
  // don't need? #include <avr/pgmspace.h> needed for strcpy_P, maybe already in build path?

class MenuState {
  public:
    MenuState(uint8_t menu_size, PGM_P * menu_labels); 
    void  printMenu();
  private:
    PGM_P * _menu_labels;
    uint8_t  _menu_size;
};     //  PGM_P same as const char *, so _menu_labels clearly pointer to pointer;
          // not clear here that it will construct an array of them
                 
MenuState::MenuState(uint8_t menu_size, PGM_P * menu_labels){
  this->_menu_size = menu_size;
  this->_menu_labels = menu_labels;
}  // end constructor
  
void MenuState::printMenu(){ 
  char buf[20]; 
        // for loops thru the ptrs in _menu_labels[], puts each one in buf and prints
         // const char * is same as PGM_P
  for ( uint8_t i = 0 ; i < _menu_size; i++ ) {
    // same as loop in setup but using _menu_labels[] one by one (instance's array of ptrs
    // created @ instantiation of class)  vs. the array of them MAIN_MENU_LABELS in setup
    const char * menu_pgm_ptr = (PGM_P) pgm_read_word(&_menu_labels[i]); // had =(PGM_P)__ cast
      // @ first worked OK without it, but then not
    strcpy_P(buf,menu_pgm_ptr);  //  pgm_read_word reads the 16 bit addr from (ref)
    Serial.println(buf);  // and what's there are array names in progmem that strcpy_P can use
  }  // end for
} // end printMenu method

const uint8_t MAIN_MENU_SIZE = 7; // main_menu_x constants are array names, ptrs to the strings
const char main_menu_1[] PROGMEM = "Sampling";
const char main_menu_2[] PROGMEM = "Next Coll.";
const char main_menu_3[] PROGMEM = "Edit Collect.";
const char main_menu_4[] PROGMEM = "File Ops.";
const char main_menu_5[] PROGMEM = "Review Current";
const char main_menu_6[] PROGMEM = "Diagnostics";
const char main_menu_7[] PROGMEM = "Power off";

  // here we store the array names (pointers) in another array in progmem
PGM_P const MAIN_MENU_LABELS[MAIN_MENU_SIZE] PROGMEM = {main_menu_1,main_menu_2,main_menu_3,main_menu_4,main_menu_5,main_menu_6,main_menu_7};
// same as const char * const MAIN_MENU_LABELS[MAIN_MENU_SIZE] PROGMEM = 
// {main_menu_1,main_menu_2,main_menu_3,main_menu_4,main_menu_5,main_menu_6,main_menu_7};

// MenuState instance creation shows elements of MAIN_MENU_LABELS are ptrs to ptrs
MenuState testMenu(MAIN_MENU_SIZE, (PGM_P *) MAIN_MENU_LABELS);  // @ 1st compiled without cast, then not

void setup() 
{
  char buf[20];
  Serial.begin(9600);
  // I can print out a single menu item (1st string) using its array name like this:
  strcpy_P(buf,main_menu_1);
  Serial.println(buf);
  
  // I can get the second string in array using MAIN_MENU_LABELS pointers
  Serial.println("second element, from MAIN_MENU_LABELS[1]");
  // MAIN_MENU_LABELS[1] should be the pointer to main_menu_2 (ptr) in PROGMEM
  const char * menu_pgm_ptr = (PGM_P) pgm_read_word(&(MAIN_MENU_LABELS[1])); // worked w/o cast then didn't
  // So menu_pgm_ptr is a pointer to a string in program space.
  strcpy_P(buf,menu_pgm_ptr);
  Serial.println(buf); // And that works too! all attempts to print (_ptr) directly failed
  
  Serial.println("Now the hard way, print all from TestMenu object method");
  testMenu.printMenu();
}  // end setup

void loop() {}
