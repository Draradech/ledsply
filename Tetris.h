typedef struct
{
  uint8_t delay;
  boolean triggered;
  boolean trigger;
} SInputState;

typedef struct
{
    int8_t   x;
    int8_t   y;
    uint8_t  type;
    uint8_t  rotation;
    uint8_t  dropdelay;
} SPieceState;

typedef enum
{
  eLeft,
  eRight,
  eRotateLeft,
  eRotateRight,
  eSlide,
  eDrop,
  eStart,
  eMode,
  eNumOfInputTypes
} EInputType;

class Tetris
{
  public:
    void init(uint8_t width, uint8_t height);
    void updateInput(boolean left, boolean right, boolean rotateLeft, boolean rotateRight, boolean slide, boolean drop, boolean start, boolean mode);
    void step(uint16_t seed);
    void render(CRGB* leds);
    boolean gameRunning(void);
    
  private:
    boolean      movePiece(int8_t dx, int8_t dy);
    boolean      rotPiece(int8_t rot);
    boolean      pieceFits(int8_t dx, int8_t dy, int8_t rot);
    void         dropPiece(void);
    void         freezePiece(void);
    void         newPiece(void);
    void         newGame(uint16_t seed);
    void         endGame(void);
    void         shuffle(void);
  
    CRGB*        m_pColors;
    SInputState* m_pInputState;
    uint8_t*     m_pField;

    uint8_t*     m_pPieces;
    uint8_t      m_pieceIndex;
    uint32_t     m_points;
    uint32_t     m_points0;
    uint32_t     m_points1;
    uint32_t     m_pointsHigh0;
    uint32_t     m_pointsHigh1;
    uint8_t      m_lines;
    boolean      m_gameRunning;
    
    SPieceState  m_pieceState;
    uint8_t      m_width;
    uint8_t      m_height;
    
    uint8_t      m_firstdelay;
    uint8_t      m_delay;
    uint8_t      m_dropdelay;
    int16_t      m_scrollOffset;
    uint8_t      m_scrollDelay;
    int8_t       m_textLen;
    
    uint8_t      m_mode;
    
    char         m_text[32];
};
