
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define JUMP_PIN 2

Adafruit_SSD1306 display(-1);

float posY = 15;
float velocity = 0;
const float gravity = 0.01;
const float jump_height = 0.4;

const int debounceTime = 200;

static long last_jump;

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

  display.drawRect(20, posY, 4, 4, WHITE);

  if (posY < 0 || posY > 28) {
    gameMode = 2;
    resetBird();
  }
}

void startFrame() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("START!");
}

void deadFrame() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("DEAD!");
}

void resetBird() {
  posY = 15;
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

