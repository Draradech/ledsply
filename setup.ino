void setup()
{
  brightIndex = 12;
  
  clock.rate(REFRESHRATE);
  
  LEDS.addLeds<WS2811, LED_OUT, GRB>(leds, LEDS_X * LEDS_Y);
  LEDS.setBrightness(bright[brightIndex]);

  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT);
  
  tetris.init(LEDS_X, LEDS_Y);

  rainbow.init(LEDS_X, LEDS_Y);
  
  matrix.init(LEDS_X, LEDS_Y);
}
