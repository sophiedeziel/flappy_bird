class Pipe
{
  public:
    int height;
    float posX = 128;

    Pipe() {
      setHeight();
    }

    void nextFrame() {
      posX += -0.5;
    }

    void reinitialize() {
      setHeight();
      posX = 128;
    }
  private:

  void setHeight(){
    height = random(48) + 15;
  }
};
