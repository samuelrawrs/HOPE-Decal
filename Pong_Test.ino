#define button1 13
#define button2 12
#define player2right 27
#define player2left 33

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

#define DIN_PIN  18 //defines data in pin connected to MOSI 18
#define CLK_PIN  5 //defines clock pin connected to sclk
#define CS_PIN   15 //defines chip select pin
#define DC_PIN   32 //defines data/command pin
#define RST_PIN  14 //defines reset pin

// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Option 1: use any pins but a little slower
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, DIN_PIN, CLK_PIN, RST_PIN);

// Option 2: must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

float p = 3.1415926;

int p1x = 128 / 2;
int p1y = 128 - 10;

int p2x = 128 / 2;
int p2y = 10;

int width = 40;
int height = 5;

float ballx = 128 / 2;
float bally = 20;
float Vx = random(-100, 100) / 50.0;
float speed = 2;
float direction = 1;
int ballRadius = 3;
int lastHit = 0; //If lasthit is 0, anyone can hit, after you hit the ball lasthit
//is your number and you can't hit it again

int numHits = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(player2right, INPUT);
  pinMode(player2left, INPUT);

  tft.begin();

  tft.fillScreen(BLACK);

  tft.fillRect(p1x, p1y, width, height, RED);
  tft.fillRect(p2x, p2y, width, height, BLUE);
  tft.fillCircle(ballx, bally, ballRadius, GREEN);

}

void movePlayer(int player, int direction) {
  //player = 1 or 2, direction = -1 for left, 1 for right
  int dx = speed * -direction;
  if (player == 1) {
    p1x = p1x + dx;
    if (p1x < 0 || p1x + width > SCREEN_WIDTH) {
      p1x = p1x - dx;
    }
    tft.fillRect(p1x, p1y, width, height, RED);
    tft.fillRect(p1x - dx, p1y, dx, height, BLACK);
    tft.fillRect(p1x + width + 1, p1y, dx, height, BLACK);
  }
  if (player == 2) {
    p2x = p2x + dx;
  }
  if (p2x < 0 || p2x + width > SCREEN_WIDTH) {
    p2x = p2x - dx;
  }
  tft.fillRect(p2x, p2y, width, height, BLUE);
  tft.fillRect(p2x - dx, p2y, dx, height, BLACK);
  tft.fillRect(p2x + width + 1, p2y, dx, height, BLACK);
}

void moveBall() {
  tft.fillCircle(ballx, bally, ballRadius, BLACK);
  ballx = ballx + Vx;
  bally = bally + speed * direction;
  tft.fillCircle(ballx, bally, ballRadius, GREEN);

  if (ballx - ballRadius <= 0 || ballx + ballRadius >= 128) {
    Vx = -Vx;
  }


//p1 hit the ball
  if (bally + ballRadius > p1y - speed * 2 && bally + ballRadius < p1y + speed * 2 && (ballx >= p1x && ballx <= p1x + width) && lastHit != 1) {
    direction = -direction;
    Vx = -(width / 2 - (ballx - p1x)) / width * 3;
    numHits += 1;
    if (numHits % 4 == 0 && numHits > 1){
      speed += .5;
    }
    lastHit = 1;
  }

  if (bally - ballRadius > p2y + height - speed * 2 && bally - ballRadius < p2y + height + speed * 2 && (ballx >= p2x && ballx <= p2x + width) && lastHit != 2) {
    direction = -direction;
    Vx = -(width / 2 - (ballx - p2x)) / width * 3;
    numHits += 1;
    if (numHits % 4 == 0 && numHits > 1){
      speed += .5;
    }
    lastHit = 2;
  }

  

  if (bally > 128) {
    delay(3000);
    while (analogRead(button1) > 2000 && analogRead(button2) > 2000 && analogRead(player2left) > 2000 && analogRead(player2right) > 2000) {
      delay(500);
    }
    ballx = 128 / 2;
    bally = 128/2;
    speed = 2;
    direction = 1;
    numHits = 0;
    lastHit = 0;
  }

  if (bally < 0) {
    delay(3000);
    while (analogRead(button1) > 2000 && analogRead(button2) > 2000  && analogRead(player2left) > 2000 && analogRead(player2right) > 2000){
      delay(500);
    }
    ballx = 128 / 2;
    bally = 128/2;
    speed = 2;
    direction = -1;
    numHits = 0;
    lastHit = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (analogRead(button1) < 2000) {
    movePlayer(1, 1);
  }
  if (analogRead(button2) < 2000) {
    movePlayer(1, -1);
  }
  if (analogRead(player2right) < 2000) {
    movePlayer(2, 1);
  }
  if (analogRead(player2left) < 2000) {
    movePlayer(2, -1);
  }
  moveBall();
  delay(20);
}
