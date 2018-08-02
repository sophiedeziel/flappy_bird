#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define JUMP_PIN 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(4);

const float gravity     = 0.03;
const float jump_height = 0.8;
const float initialY    = 32;
const int debounceTime  = 200;

long last_jump;
float posY     = initialY;
float velocity = 0;

// game modes:
// 0: start
// 1: jeu
// 2: dead
int gameMode = 0;

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(JUMP_PIN), jump, FALLING);
}

void loop() {
  display.clearDisplay();

  switch (gameMode) {
    case 0:
      startFrame();
      break;
    case 1:
      gameNextFrame();
      break;
    case 2:
      deadFrame();
      break;
  }
  display.display();
}

void gameNextFrame() {
  velocity += gravity;
  posY += velocity;

  display.drawFastHLine(0, 15, 128, WHITE);
  display.drawRect(20, posY, 6, 6, WHITE);

  if (posY < 16 || posY > 58) {
    gameMode = 2;
    resetBird();
  }
}

void startFrame() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println("START!");
}

void deadFrame() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println("DEAD!");
}

void resetBird() {
  posY = initialY;
  velocity = 0;
}

void jump() {
  gameMode = 1;
  if (millis() - last_jump > debounceTime) {
    last_jump = millis();
    velocity = -jump_height;
    Serial.println("Jump!");
  }
}

