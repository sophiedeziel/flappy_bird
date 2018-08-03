class Pipe
{
  public:
    int height;
    float posX = 128;

    Pipe() {
      height = random(48) + 15;
    }

    void nextFrame() {
      posX -= 0.5;
    }
};
