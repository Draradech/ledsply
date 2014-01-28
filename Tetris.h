class Tetris
{
  public:
    void init(uint8_t width, uint8_t height, uint8_t refreshrate);
    void step(boolean left, boolean right, boolean rotleft, boolean rotright, boolean slide, boolean drop, boolean start);
    void render(CRGB* leds);
    boolean gameRunning();
    
  private:
    uint8_t* m_field;
    uint8_t m_rate;
    uint8_t m_width;
    uint8_t m_height;
    static CRGB colors[];
};
