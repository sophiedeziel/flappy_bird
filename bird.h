class Bird
{
  public:
    const float initialY    = 32;
    const float jump_height = 0.8;
    const float gravity     = 0.03;

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
