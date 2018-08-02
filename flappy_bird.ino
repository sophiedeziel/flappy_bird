
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define JUMP_PIN 2

Adafruit_SSD1306 display(-1);

float posY = 15;
float velocity = 0;
const float gravity = 0.008;
const float jump_height = 0.4;

const int debounceTime = 200;

static long last_jump;

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(JUMP_PIN), jump, FALLING);
}

void loop() {
  gameNextFrame();
}

void gameNextFrame() {
  velocity += gravity;
  posY += velocity;

  display.clearDisplay();
  display.drawRect(20, posY, 4, 4, WHITE);
  display.display();
}

void jump() {
  if (millis() - last_jump > debounceTime) {
    last_jump = millis();
    velocity = -jump_height;
    Serial.println("Jump!");
  }
}

