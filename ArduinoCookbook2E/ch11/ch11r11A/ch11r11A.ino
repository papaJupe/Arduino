/*
  TellyBounce
*/

// define the edges of the screen:
const int HEIGHT = 25;    // the number of text rows
const int WIDTH  = 38;    // the number of characters in a row
const int LEFT   =  0;    // useful constants derived from the above
const int RIGHT  = WIDTH -1;
const int TOP    = 0;
const int BOTTOM = HEIGHT-1;

const byte BALL  = 'o';      // character code for ball
const byte ESC = 0x1B;       // ASCII escape character used in TellyMate commands

int ballX = WIDTH/2;          // X position of the ball
int ballY = HEIGHT/2;         // Y position of the ball
int ballDirectionY = 1;       // X direction of the ball
int ballDirectionX = 1;       // Y direction of the ball

// this delay moves ball across the 38-character screen in just under 4 seconds
long interval = 100;          

void setup()
{
  Serial.begin(57600);   // 57k6 baud is default TellyMate speed
  clear();               // clear the screen
  cursorHide();          // turn cursor off
}

void loop()
{
  moveBall();
  delay(interval);
}

void moveBall() {
  // if the ball goes off the top or bottom, reverse its Y direction
  if (ballY == BOTTOM || ballY == TOP)
    ballDirectionY = -ballDirectionY;

  // if the ball goes off the left or right, reverse its X direction
  if ((ballX == LEFT) || (ballX == RIGHT))
    ballDirectionX = -ballDirectionX;

  // clear the ball's previous position
  showXY(' ', ballX, ballY);

  // increment the ball's position in both directions
  ballX = ballX + ballDirectionX;
  ballY = ballY + ballDirectionY;

  // show the new position
  showXY(BALL, ballX, ballY);
}

// TellyMate helper functions

void clear( )  // clear the screen
{ // <ESC>E
  Serial.write(ESC);
  Serial.write('E');
}

void setCursor( int col, int row) // set the cursor
{ // <ESC>Yrc
  Serial.write(ESC);
  Serial.write('Y' ) ;
  Serial.write((unsigned char)(32 + row)) ;
  Serial.write((unsigned char)(32 + col)) ;
}

void cursorShow( )
{ // <ESC>e
  Serial.write(ESC) ;
  Serial.write('e') ;
}

void cursorHide()
{ // <ESC>f
  Serial.write(ESC) ;
  Serial.write('f' ) ;
}

void showXY( char ch, int x, int y){
  // display the given character at the screen x and y location
  setCursor(x,y);
  Serial.write(ch);
}