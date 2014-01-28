class Rainbow
{
  public:
    void init(uint8_t width, uint8_t height, uint8_t refreshrate);
    void step();
    void render(CRGB* leds);
    
  private:
    uint8_t* m_hue;
    uint8_t  m_rate;
    uint8_t  m_width;
    uint8_t  m_height;
};
