#include <avr/eeprom.h>
#include <FastLED.h>
#include "Tetris.h"
#include "Tetris_p.h"
#include "Font.h"

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

void Tetris::init(uint8_t width, uint8_t height)
{
  m_width = width;
  m_height = height;

  m_pField = new uint8_t[m_height * m_width];
  memset(m_pField, 0, m_height * m_width);

  m_pPieces = new uint8_t[7];

  m_pColors    = new CRGB[8];
  m_pColors[0] = CRGB(  0,   0,   0);
  m_pColors[1] = CRGB(  0, 255, 255);
  m_pColors[2] = CRGB(127,   0, 255);
  m_pColors[3] = CRGB(255, 127,   0);
  m_pColors[4] = CRGB(  0,   0, 255);
  m_pColors[5] = CRGB( 64, 255,   0);
  m_pColors[6] = CRGB(192,   0,   0);
  m_pColors[7] = CRGB(255, 255,   0);
  
  m_pInputState = new SInputState[eNumOfInputTypes];
  
  m_points = 0;
  m_firstdelay = 8;
  m_delay = 2;
  
  m_pointsHigh = eeprom_read_dword(0);
  
  sprintf(m_text, "High:%ld", m_pointsHigh);
  m_textLen = strlen(m_text);
  m_scrollOffset = -m_width;
}

void Tetris::updateInput(boolean left, boolean right, boolean rotateLeft, boolean rotateRight, boolean slide, boolean drop, boolean start)
{
  for(EInputType type = (EInputType) 0; type < eNumOfInputTypes; type = (EInputType)(type + 1))
  {
    boolean key = type == eLeft ? left : type == eRight ? right : type == eRotateLeft ? rotateLeft : type == eRotateRight ? rotateRight : type == eSlide ? slide : type == eDrop ? drop : start;
    if(key)
    {
      if(!m_pInputState[type].trigger)
      {
        if(!m_pInputState[type].delay)
        {
          m_pInputState[type].trigger = true;
          if(!m_pInputState[type].triggered)
          {
            m_pInputState[type].triggered = true;
            m_pInputState[type].delay = (type == eLeft || type == eRight || type == eSlide) ? m_firstdelay : 2 * m_firstdelay;
          }
          else
          {
            m_pInputState[type].delay = m_delay;
          }
        }
        else
        {
          m_pInputState[type].delay--;
        }
      }
      else
      {
        m_pInputState[type].delay = m_delay;
      }
    }
    else
    {
      m_pInputState[type].delay = 0;
      m_pInputState[type].triggered = false;
      m_pInputState[type].trigger = false;
    }  
  }
}

void Tetris::step(uint16_t seed)
{
  if(!m_gameRunning)
  {
    if(m_pInputState[eStart].trigger)
    {
      newGame(seed);
    }
    else
    {
      if(m_scrollDelay)
      {
        m_scrollDelay--;
      }
      else
      {
        m_scrollOffset++;
        m_scrollDelay = 4;
      }
      if(m_scrollOffset > 6 * m_textLen)
      {
        m_scrollOffset = -m_width;
      }
      return;
    }
  }
  
  if(m_pieceState.dropdelay)  m_pieceState.dropdelay--;
  else if(!m_pieceState.type) newPiece();
  else if(movePiece(0, -1))   {if(!pieceFits(0, -1, 0)) m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay); else m_pieceState.dropdelay = m_dropdelay;}
  else                        freezePiece();

  if(!m_pieceState.type) return;

  if(m_pInputState[eRotateRight].trigger && rotPiece(1)) {if(!pieceFits(0, -1, 0)) m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay);                                            m_pInputState[eRotateRight].trigger = false; tone(4, 220, 30);}
  if(m_pInputState[eRotateLeft].trigger && rotPiece(-1)) {if(!pieceFits(0, -1, 0)) m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay);                                            m_pInputState[eRotateLeft].trigger = false; tone(4, 220, 30);}
  
  if(m_pInputState[eLeft].trigger &&  movePiece(-1,  0)) {if(!pieceFits(0, -1, 0)) m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay);                                            m_pInputState[eLeft].trigger = false; tone(4, 220, 30);}
  if(m_pInputState[eRight].trigger && movePiece( 1,  0)) {if(!pieceFits(0, -1, 0)) m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay);                                            m_pInputState[eRight].trigger = false; tone(4, 220, 30);}
  if(m_pInputState[eSlide].trigger && movePiece( 0, -1)) {if(!pieceFits(0, -1, 0)) m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay); else m_pieceState.dropdelay = m_dropdelay; m_pInputState[eSlide].trigger = false; tone(4, 220, 30);}

  if(m_pInputState[eDrop].trigger) {tone(4, 220, 30); dropPiece(); m_pInputState[eDrop].trigger = false; }
}

boolean Tetris::movePiece(int8_t dx, int8_t dy)
{
  if(pieceFits(dx, dy, 0)) {m_pieceState.x += dx; m_pieceState.y += dy; return true;}
  return false;
}

boolean Tetris::rotPiece(int8_t rot)
{
  if(pieceFits(   0,  0, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; return true;}
  if(pieceFits( rot,  0, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.x += rot; return true;}
  if(pieceFits(-rot,  0, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.x -= rot; return true;}
  if(pieceFits(   0, -1, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.y -= 1;   return true;}
  if(pieceFits( rot, -1, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.x += rot; m_pieceState.y -= 1; return true;}
  if(pieceFits(-rot, -1, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.x -= rot; m_pieceState.y -= 1; return true;}
  if(m_pieceState.type == 1 && pieceFits( 2 * rot,  0, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.x += 2 * rot; return true;}
  if(m_pieceState.type == 1 && pieceFits(-2 * rot,  0, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.x -= 2 * rot; return true;}
  if(m_pieceState.type == 1 && pieceFits(       0, -2, rot)) {m_pieceState.rotation = (m_pieceState.rotation + rot + 4) % 4; m_pieceState.y -= 2;   return true;}
  return false;
}

boolean Tetris::pieceFits(int8_t dx, int8_t dy, int8_t rot)
{
  if(!m_pieceState.type) return false;
  for(uint8_t px = 0; px < 4; px++)
  {
    for(uint8_t py = 0; py < 4; py++)
    {
      if(pgm_read_byte(&(pieces[m_pieceState.type - 1][(m_pieceState.rotation + rot + 4) % 4][py][px])))
      {
        int8_t x = m_pieceState.x + px + dx;
        int8_t y = m_pieceState.y - py + dy;
        uint16_t index = y * m_width + x;
        if(x < 0 || x >= m_width || y < 0 || y >= m_height || m_pField[index]) return false;
      }
    }
  }
  return true;
}

void Tetris::dropPiece(void)
{
  while(movePiece(0, -1))
  {
    m_points++;
  }
  freezePiece();
}

void Tetris::freezePiece(void)
{
  for(uint8_t px = 0; px < 4; px++)
  {
    for(uint8_t py = 0; py < 4; py++)
    {
      if(pgm_read_byte(&(pieces[m_pieceState.type - 1][m_pieceState.rotation][py][px])))
      {
        int8_t x = m_pieceState.x + px;
        int8_t y = m_pieceState.y - py;
        if(x >= 0 && x < m_width && y >= 0 && y < m_height)
        {
          uint16_t index = y * m_width + x;
          m_pField[index] = m_pieceState.type;
        }
      }
    }
  }
  uint8_t lines = 0;
  for(int8_t y = 0; y < m_height; y++)
  {
    if(lines)
    {
      for(uint8_t x = 0; x < m_width; x++)
      {
        uint16_t index = y * m_width + x;
        if(y + lines >= m_height)
        {
          m_pField[index] = 0;
        }
        else
        {
          m_pField[index] = m_pField[index + lines * m_width];
        }
      }
    }
    uint8_t num = 0;
    for(uint8_t x = 0; x < m_width; x++)
    {
      uint16_t index = y * m_width + x;
      if(m_pField[index]) num++;
    }
    if(num == 10)
    {
      lines++;
      y--;
    }
  }
  if(lines)
  {
    tone(4, 440, 40 * lines);
  }
  m_points += lines * 100;
  if(lines == 4) m_points += 400;
  m_lines += lines;
  m_dropdelay = MAX(2, 25 - m_lines / 5);
  
  m_pieceState.dropdelay = MAX(m_firstdelay, m_dropdelay);
  m_pieceState.type = 0;
}

void Tetris::newPiece(void)
{
  m_pieceState.x = 3;
  m_pieceState.y = 21;
  m_pieceState.rotation = 0;
  m_pieceState.dropdelay = m_dropdelay;
  
  if(m_pieceIndex == 7)
  {
    shuffle();
  }
  
  for(EInputType type = (EInputType) 0; type < eNumOfInputTypes; type = (EInputType)(type + 1))
  {
     m_pInputState[type].triggered = false;
  }

  m_pieceState.type = m_pPieces[m_pieceIndex];
  m_pieceIndex++;
  
  if(!pieceFits(0, 0, 0))
  {
    endGame();
  }
}

void Tetris::shuffle()
{
    for(uint8_t i = 0; i < 7; i++)
    {
      uint8_t j = random8(i+1);
      m_pPieces[i] = m_pPieces[j];
      m_pPieces[j] = i + 1;
    }
    m_pieceIndex = 0;
}

void Tetris::newGame(uint16_t seed)
{
    memset(m_pField, 0, m_height * m_width);
    m_dropdelay = 25;
    m_lines = 0;
    m_points = 0;
    
    random16_set_seed(seed);
    shuffle();
    m_pieceIndex = 4;
    
    m_gameRunning = true;
}

void Tetris::endGame(void)
{
  m_gameRunning = false;
  
  if(m_points > m_pointsHigh)
  {
      m_pointsHigh = m_points;
      eeprom_write_dword(0, m_pointsHigh);
  }

  sprintf(m_text, "Last:%ld High:%ld", m_points, m_pointsHigh);
  m_textLen = strlen(m_text);
  m_scrollOffset = -m_width;
}

boolean Tetris::gameRunning(void)
{
  return m_gameRunning;
}


void Tetris::render(CRGB* leds)
{
  if(m_gameRunning)
  {
    for(uint8_t y = 0; y < m_height; y++)
    {
      for(uint8_t x = 0; x < m_width; x++)
      {
        uint16_t index = y * m_width + x;
        
        leds[index] = m_pColors[m_pField[index]];
      }
    }
    if(!m_pieceState.type) return;
    for(uint8_t px = 0; px < 4; px++)
    {
      for(uint8_t py = 0; py < 4; py++)
      {
        if(pgm_read_byte(&(pieces[m_pieceState.type - 1][m_pieceState.rotation][py][px])))
        {
          int8_t x = m_pieceState.x + px;
          int8_t y = m_pieceState.y - py;
          if(x >= 0 && x < m_width && y >= 0 && y < m_height)
          {
            uint16_t index = y * m_width + x;
            leds[index] = m_pColors[m_pieceState.type];
          }
        }
      }
    }
  }
  else
  {
    for(uint8_t y = 0; y < m_height; y++)
    {
      for(uint8_t x = 0; x < m_width; x++)
      {
        uint16_t index = y * m_width + x;
        
        leds[index] = m_pColors[m_pField[index]];
        leds[index] %= 100;
      }
    }

    for(uint8_t x = 0; x < m_width; ++x)
    {
      int8_t ch = (m_scrollOffset + x) / 6;
      int8_t nch = (m_scrollOffset + x + 1) / 6;
      if((m_scrollOffset + x) >= 0 && ch >= 0 && ch == nch && ch < m_textLen)
      {
        char c = m_text[ch];
        if(c >= 33 && c <= 127)
        {
          uint8_t col = pgm_read_byte(&(font[c - 32][(m_scrollOffset + x) % 6]));
          for(uint8_t y = 0; y < 8; ++y)
          {
            if(col & (1 << y))
            {
              uint16_t index = (m_height - y - 2) * m_width + x;
              leds[index] = CRGB::White;
            }
          }
        }
      }
    }
  }
}

