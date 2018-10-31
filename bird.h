

class Bird
{
  public:
    const float initialY    = 32;
    const float jump_height = 2.3;
    const float gravity     = 0.15;
    const int   posX = 20;

    float posY;
    float velocity = 0;

    Bird() {
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
      Adafruit_SSD1306 display(4);
      display.setRotation(1);
      display.drawRect(posX, posY, 6, 6, WHITE);
    }
};
