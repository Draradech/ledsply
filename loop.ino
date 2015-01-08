void loop()
{
  if (clock.expired())
  {
    ps2x.read_gamepad();

    switch(mode)
    {
      case modeTetris:
        tetris.updateInput(
          ps2x.Button(PSB_PAD_LEFT),     // left
          ps2x.Button(PSB_PAD_RIGHT),    // right
          ps2x.Button(PSB_SQUARE),       // rotate left
          ps2x.Button(PSB_TRIANGLE),     // rotate right
          ps2x.Button(PSB_PAD_DOWN),     // slide down
          ps2x.Button(PSB_PAD_UP),       // drop down
          ps2x.Button(PSB_START)         // start, pause
        );
        tetris.step(seed);
        tetris.render(leds);
        break;
      case modeRainbow:
        rainbow.step();
        rainbow.render(leds);
        break;
      case modeMatrix:
        matrix.step();
        matrix.render(leds);
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

    if(ps2x.Button(PSB_R1) && bright < 255)
    {
      bright++;
      LEDS.setBrightness(bright);
    }

    if(ps2x.Button(PSB_L2))
    {
      for(uint8_t i = 0; i < 10; i++)
      {
        int8_t bright = percent - i * 10;
        bright = constrain(bright, 0, 10);
        leds[i] = CHSV(0, 0, bright * 25);
      }
    }

    LEDS.show();

    percent = clock.value() * 100 / clock.base();
  }
  seed++;
}
