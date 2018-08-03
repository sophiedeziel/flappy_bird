class Pipe
{
  public:
    int height;
    float posX;

    Pipe(int initialX) {
      posX = initialX;
      setHeight();
    }

    void nextFrame() {
      posX += -0.5;
    }

    void reinitialize(float newPosition) {
      setHeight();
      posX = newPosition;
    }

  private:

    void setHeight() {
      height = random(35);
    }
};
