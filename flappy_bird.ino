#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#include "bird.h"
#include "pipe.h"

#define JUMP_PIN 2
#define NUM_PIPES 3
#define BEST_SCORE_AT_ADDR 0

Adafruit_SSD1306 display(128, 64, &Wire , 4);

const int debounceTime  = 200;
const int voltSampleTime = 300;
long last_jump;
long last_volt = millis();
int batteryFrame = 0;


uint8_t score = 0;

int gameXOffset = 0;
int gameYOffset = 16;
int gameWidth   = 64;
int gameHeight  = 128 - gameYOffset;

uint8_t bestScoreAT = 0;
uint8_t bestScoreToday = 0;


Bird bird(&display);

Pipe pipes[NUM_PIPES] = {
  Pipe(0, &display),
  Pipe(0, &display),
  Pipe(0, &display)
};

// game modes:
// 0: start
// 1: jeu
// 2: dead
int gameMode = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);
  pinMode(JUMP_PIN,  INPUT);
  attachInterrupt(digitalPinToInterrupt(JUMP_PIN), jump, RISING);
  display.setRotation(1);
  initPipes();

  EEPROM.get(BEST_SCORE_AT_ADDR, bestScoreAT);
  Serial.println(bestScoreAT);
}

void loop() {
  display.clearDisplay();

  switch (gameMode) {
    case 0:
      showBattery();
      startFrame();
      break;
    case 1:
      showBattery();
      showScore();
      gameNextFrame();
      break;
    case 2:
      showBattery();
      showScore();
      deadFrame();
      break;
  }
  display.display();
}

void gameNextFrame() {
  display.drawRect(gameXOffset, gameYOffset, gameWidth, gameHeight, WHITE);

  birdMove();
  checkCollisions();
}

void checkCollisions() {
  for (int i = 0; i < NUM_PIPES; i++) {
    pipeMove(pipes[i]);

    if (pipes[i].posX <= bird.posX && (pipes[i].posX + 6) >= bird.posX) {
      if (bird.posY > gameYOffset + pipes[i].height && bird.posY < pipes[i].holeSize + pipes[i].height) {

      } else {
        gameOver();
      }
    }
    if ((pipes[i].posX + 6) == bird.posX) {
      score++;
    }
  }
}

void birdMove() {
  bird.nextFrame();

  if (bird.posY < gameYOffset || bird.posY > gameHeight + gameYOffset) {
    gameOver();
  }

  bird.draw();
}

void pipeMove(Pipe &pipe) {
  pipe.nextFrame();
  pipe.draw();
}

void startFrame() {
  titleFrame("START");
}

void deadFrame() {
  titleFrame("Game over!");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 95);
  display.print("Best:  ");
  display.println(bestScoreAT);
  display.setCursor(0, 110);
  display.print("Today: ");
  display.println(bestScoreToday);
}

void titleFrame(String text) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, gameHeight / 2 );
  display.println(text);
}

void showBattery() {
  int volts = readVcc();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(40, 0);
  display.drawRect(50, 0, 14, 8, WHITE);
  display.drawRect(48, 2, 2, 4, WHITE);

  if (volts > 4500) {
    batteryFrame++;
    if (batteryFrame > 14) {
      batteryFrame = 0;
    }

    display.fillRect(50, 0, batteryFrame, 8, WHITE);
  } else {
    int level = map(volts, 3600, 4000, 0, 14);
    display.fillRect(50, 0, level, 8, WHITE);
  }
}

void showScore() {
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(score);
}

float readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  float result = (high << 8) | low;

  result = 1.1 * 1023.0 * 1000.0 / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000

  return result;
}

void gameOver() {
  gameMode = 2;
  bird.reinitialize();
  initPipes();

  if (bestScoreAT < score) {
    bestScoreAT = score;
  }

  if (bestScoreToday < score) {
    bestScoreToday = score;
  }

  EEPROM.put(BEST_SCORE_AT_ADDR, bestScoreAT);
}

void initPipes() {
  float nextPipePosition = gameWidth;
  for (int i = 0; i < NUM_PIPES; i++) {
    pipes[i].reinitialize(nextPipePosition + random(10));
    nextPipePosition += pipes[i].pipeSpace;
  }
}

void jump() {
  if (millis() - last_jump > debounceTime) {
    last_jump = millis();

    switch (gameMode) {
      case 0:
        gameMode = 1;
        break;
      case 1:
        bird.jump();
        break;
      case 2:
        gameMode = 1;
        score = 0;
        break;
    }
  }
}
