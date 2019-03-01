

class Bird
{
  public:
    const float initialY    = 32;
    const float jump_height = 2.3;
    const float gravity     = 0.15;
    const int   posX = 20;
    const Adafruit_SSD1306 *display;

    float posY;
    float velocity = 0;

    Bird(const Adafruit_SSD1306* _display) {
      display = _display;
      posY     = initialY;
      velocity = 0;
    }

    void nextFrame() {
      velocity += gravity;
      posY     += velocity;
    }

    void reinitialize() {
      posY = initialY;
      velocity = 0;
    }

    void jump() {
      velocity = -jump_height;
    }

    void draw() {
      display->drawRect(posX, posY, 6, 6, WHITE);
    }
};
