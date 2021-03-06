/*************************/
/* Includes              */
/*************************/
#include <FastLED.h>
#include <PS2X_lib.h>
#include "Clock4.h"
#include "Tetris.h"
#include "Rainbow.h"
#include "Matrix.h"

/*************************/
/* Defines               */
/*************************/
#define REFRESHRATE 50

#define LED_OUT 6
#define LEDS_X 10
#define LEDS_Y 20

#define PS2_DAT A2
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 12

typedef enum EMode
{
  modeRainbow = 0,
  modeMatrix,
  modeTetris,
  modeNumberOfModes
} EMode;

/*************************/
/* Globals               */
/*************************/
CRGB    leds[LEDS_X * LEDS_Y];
PS2X    ps2x;
Clock4  clock;
Tetris  tetris;
Rainbow rainbow;
Matrix  matrix;
uint8_t mode;
uint8_t brightIndex;
uint16_t seed;
uint8_t percent;
uint8_t brightnessPressed;
