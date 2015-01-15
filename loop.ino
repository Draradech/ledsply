uint8_t bright[] = {2,3,4,6,9,14,21,31,47,72,109,167,255};

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
          ps2x.Button(PSB_SQUARE) || ps2x.Button(PSB_CROSS),       // rotate left
          ps2x.Button(PSB_TRIANGLE) || ps2x.Button(PSB_CIRCLE),     // rotate right
          ps2x.Button(PSB_PAD_DOWN),     // slide down
          ps2x.Button(PSB_PAD_UP),       // drop down
          ps2x.Button(PSB_START),         // start, pause
          ps2x.Button(PSB_SELECT)
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
    
   if(ps2x.Button(PSB_L1))
   {
    if(!brightnessPressed)
    {
      brightnessPressed = 1;
      if(brightIndex > 1) brightIndex--;
      LEDS.setBrightness(bright[brightIndex]);
    }
   }
   else if(ps2x.Button(PSB_R1))
   {
    if(!brightnessPressed)
    {
      brightnessPressed = 1;
      if(brightIndex < 12) brightIndex++;
      LEDS.setBrightness(bright[brightIndex]);
    }
   }
   else
   {
     brightnessPressed = 0;
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
