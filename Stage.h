#ifndef __STAGE_H
#define __STAGE_H

class Stage{
  public:
  virtual void render(Adafruit_SSD1306 * display) = 0;
  virtual void encoderRotated(int deltaSteps) = 0;
  virtual void encoderButtonPressed() = 0;
  virtual bool isStageFinished() = 0;
};

#endif