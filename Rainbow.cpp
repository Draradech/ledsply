#include <FastLED.h>
#include "Rainbow.h"

void Rainbow::init(uint8_t width, uint8_t height, uint8_t refreshrate)
{
  m_rate = refreshrate;
  m_width = width;
  m_height = height;
  m_hue = new uint8_t[m_height * m_width];

  for(uint8_t y = 0; y < m_height; y++)
  {
    for(uint8_t x = 0; x < m_width; x++)
    {
      uint16_t index = y * m_width + x;
      m_hue[index] = index * 255 / 147;
    }
  }
}

void Rainbow::step()
{
  for(uint8_t y = 0; y < m_height; y++)
  {
    for(uint8_t x = 0; x < m_width; x++)
    {
      uint16_t index = y * m_width + x;
      m_hue[index]++;
    }
  }
}

void Rainbow::render(CRGB* leds)
{
  for(uint8_t y = 0; y < m_height; y++)
  {
    for(uint8_t x = 0; x < m_width; x++)
    {
      uint16_t index = y * m_width + x;
      leds[index] = CHSV(m_hue[index], 255, 255);
    }
  }
}

