typedef struct
{
  int8_t x, y;
  uint8_t delay;
  uint8_t delaySetting;
} SParticle;

class Matrix
{
  public:
    void init(uint8_t width, uint8_t height);
    void step();
    void render(CRGB* leds);
    
  private:
    void initParticle(uint8_t i);
    uint8_t  m_width;
    uint8_t  m_height;
    SParticle particle[10];
};
