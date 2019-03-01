class Pipe
{
  public:
    int height;
    int holeSize = 32;
    float posX;

    const int pipeSpace = 30;
    const Adafruit_SSD1306 *display;

    Pipe(int initialX, const Adafruit_SSD1306* _display) {
      display = _display;
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

      display->drawRect(posX, 16, 6, height, WHITE);
      display->drawRect(posX, height + holeSize, 6, 100, WHITE);
    }

  private:

    void setHeight() {
      height = random(80) + 10;
    }
};
