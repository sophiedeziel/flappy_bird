#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bird.h"
#include "pipe.h"

#define JUMP_PIN 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Bird bird;
Pipe pipe;

Adafruit_SSD1306 display(4);

const int debounceTime  = 200;
long last_jump;

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
  bird.nextFrame();
  pipe.nextFrame();

  display.drawRect(pipe.posX, 16, 4, pipe.height, WHITE);
  display.drawRect(pipe.posX, pipe.height + 32, 4, 64, WHITE);

  display.drawRect(0, 16, 128, 48, WHITE);
  display.drawRect(20, bird.posY, 6, 6, WHITE);

  if (bird.posY < 17 || bird.posY > 58) {
    gameOver();
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

void gameOver() {
  gameMode = 2;
  bird.reinitialize();
}

void jump() {
  gameMode = 1;
  if (millis() - last_jump > debounceTime) {
    last_jump = millis();
    bird.jump();
  }
}


