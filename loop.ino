void loop()
{
  if (clock.expired())
  {
    LEDS.show();

    ps2x.read_gamepad();

    switch(mode)
    {
      case modeTetris:
        tetris.step(
          ps2x.Button(PSB_PAD_LEFT),                            // left
          ps2x.Button(PSB_PAD_RIGHT),                           // right
          ps2x.Button(PSB_SQUARE),                              // rotate left
          ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_CIRCLE),   // rotate right
          ps2x.Button(PSB_PAD_DOWN),                            // slide down
          ps2x.Button(PSB_CROSS),                               // drop down
          ps2x.Button(PSB_START)                                // start, pause
        );
        tetris.render(leds);
        break;
      case modeRainbow:
        rainbow.step();
        rainbow.render(leds);
        break;
    }
    
    if(ps2x.ButtonPressed(PSB_R2) && (mode != modeTetris || !tetris.gameRunning()))
    {
      mode++;
      mode %= modeNumberOfModes;
    }
    
    if(ps2x.Button(PSB_L1) && bright > 0)
    {
      bright--;
      LEDS.setBrightness(bright);
    }

    if(ps2x.Button(PSB_R1) && bright < 192)
    {
      bright++;
      LEDS.setBrightness(bright);
    }

    if(ps2x.Button(PSB_L2))
    {
      uint8_t percent = clock.value() * 100 / clock.base();
      for(uint8_t i = 0; i < 10; i++)
      {
        int8_t bright = percent - i * 10;
        bright = constrain(bright, 0, 10);
        leds[i] = CHSV(0, 0, bright * 25);
      }
    }
  }
}
