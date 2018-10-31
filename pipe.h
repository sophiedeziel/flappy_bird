class Pipe
{
  public:
    int height;
    int holeSize = 32;
    float posX;

    const int pipeSpace = 30;

    Pipe(int initialX) {
      posX = initialX;
      setHeight();
    }

    void nextFrame() {
      posX += -0.5;
      
      if (posX < -4) {
        reinitialize(pipeSpace * 3 + random(5));
      }
    }

    void reinitialize(float newPosition) {
      setHeight();
      posX = newPosition;
    }

    void draw() {
      Adafruit_SSD1306 display(4);
      display.setRotation(1);

      display.drawRect(posX, 16, 4, height, WHITE);
      display.drawRect(posX, height + holeSize, 4, 100, WHITE);
    }

  private:

    void setHeight() {
      height = random(80) + 10;
    }
};
