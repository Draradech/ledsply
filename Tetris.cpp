#include <FastLED.h>
#include "Tetris.h"

CRGB Tetris::colors[] = {CRGB(0,0,0), CRGB(255,0,0), CRGB(255,255,0), CRGB(0,255,0), CRGB(0,255,255), CRGB(0,0,255), CRGB(255,0,255), CRGB(255,255,255)};

void Tetris::init(uint8_t width, uint8_t height, uint8_t refreshrate)
{
  m_rate = refreshrate;
  m_width = width;
  m_height = height;
  m_field = new uint8_t[m_height * m_width];
  memset(m_field, 7, m_height * m_width);
}

void Tetris::step(boolean left, boolean right, boolean rotleft, boolean rotright, boolean slide, boolean drop, boolean start)
{
}

boolean Tetris::gameRunning()
{
  return false;
}


void Tetris::render(CRGB* leds)
{
  for(uint8_t y = 0; y < m_height; y++)
  {
    for(uint8_t x = 0; x < m_width; x++)
    {
      uint16_t index = y * m_width + x;
      
      leds[index] = colors[m_field[index]];
    }
  }
}

