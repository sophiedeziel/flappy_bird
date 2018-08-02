#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bird.h"

#define JUMP_PIN 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(4);

const int debounceTime  = 200;
long last_jump;

// game modes:
// 0: start
// 1: jeu
// 2: dead
int gameMode = 0;

Bird bird;

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
  bird.nextFrame();
  display.drawFastHLine(0, 15, 128, WHITE);
  display.drawRect(20, bird.posY, 6, 6, WHITE);

  if (bird.posY < 16 || bird.posY > 58) {
    gameMode = 2;
    bird.reinitialize();
  }
}

void startFrame() {
  titleFrame("START!");
}

void deadFrame() {
  titleFrame("DEAD!");
}

void titleFrame(String text) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println(text);
}

void jump() {
  gameMode = 1;
  if (millis() - last_jump > debounceTime) {
    last_jump = millis();
    bird.jump();
  }
}


