#include <FastLED.h>
#include "Matrix.h"

void Matrix::init(uint8_t width, uint8_t height)
{
  m_width = width;
  m_height = height;
  
  for(uint8_t i = 0; i < 10; i++)
  {
    initParticle(i);
  }
}

void Matrix::initParticle(uint8_t i)
{
    particle[i].y = m_height + random8(5);
    particle[i].x = random8(m_width);
    particle[i].delaySetting = random8(8, 16);
    particle[i].delay = particle[i].delaySetting;
}

void Matrix::step()
{
  for(uint8_t i = 0; i < 10; i++)
  {
    if(particle[i].delay)
    {
      particle[i].delay--;
    }
    else
    {
      particle[i].y--;
      particle[i].delay = particle[i].delaySetting;
      if(particle[i].y < 0)
      {
        initParticle(i);
      }
    }
  }
}

void Matrix::render(CRGB* leds)
{
  for(uint8_t y = 0; y < m_height; y++)
  {
    for(uint8_t x = 0; x < m_width; x++)
    {
      uint16_t index = y * m_width + x;
      leds[index] -= CRGB(8, 2, 8);
    }
  }
  for(uint8_t i = 0; i < 10; i++)
  {
      if(particle[i].y < m_height)
      {
        uint16_t index = particle[i].y * m_width + particle[i].x;
        leds[index] = CRGB::White;
      }
  }
}
