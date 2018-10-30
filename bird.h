class Bird
{
  public:
    const float initialY    = 32;
    const float jump_height = 2.0;
    const float gravity     = 0.1;

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
};
