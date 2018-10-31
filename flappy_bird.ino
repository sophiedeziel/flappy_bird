#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "bird.h"
#include "pipe.h"

#define JUMP_PIN 2
#define NUM_PIPES 3

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(4);

const int debounceTime  = 200;
long last_jump;

int score = 0;

int gameXOffset = 0;
int gameYOffset = 16;
int gameWidth = SSD1306_LCDHEIGHT;
int gameHeight = SSD1306_LCDWIDTH - gameYOffset;


Bird bird;

Pipe pipes[NUM_PIPES] = {
  Pipe(0),
  Pipe(0),
  Pipe(0)
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
  attachInterrupt(digitalPinToInterrupt(JUMP_PIN), jump, FALLING);
  display.setRotation(1);
  initPipes();
}

void loop() {
  display.clearDisplay();

  switch (gameMode) {
    case 0:
      startFrame();
      break;
    case 1:
      showScore();
      gameNextFrame();
      break;
    case 2:
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

    if (pipes[i].posX == bird.posX) {
      if (bird.posY > gameYOffset + pipes[i].height && bird.posY < pipes[i].holeSize + pipes[i].height) {
        score++;
      } else {
        gameOver();
      }
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
}

void titleFrame(String text) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, gameHeight / 2 );
  display.println(text);
}

void showScore() {
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(score);
}

void gameOver() {
  gameMode = 2;
  bird.reinitialize();
  initPipes();
}

void initPipes() {
  float nextPipePosition = gameWidth;
  for (int i = 0; i < NUM_PIPES; i++) {
    pipes[i].reinitialize(nextPipePosition + random(5));
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
