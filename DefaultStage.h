#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Stage.h"
#include "AlarmManager.h"

class DefaultStage: public Stage{
  public:
  long timeoutStart;

    void resetTimer(){
        timeoutStart = millis();
    }

  virtual void render(Adafruit_SSD1306 * display){
    if(!(millis() - timeoutStart > 7000)){
        String timeToAlarm = alarmManager.strTimeToAlarm();
        display->setTextSize(1);      // Normal 1:1 pixel scale
        display->setTextColor(WHITE); // Draw white text
        display->setCursor(0,2);
        display->println("Sleep left:");
        display->setTextSize(2);
        display->setCursor(56, 11);
        display->println(timeToAlarm);
    }
  };
  virtual void encoderRotated(int deltaSteps){};
  virtual void encoderButtonPressed(){
      timeoutStart = millis();
  };
  virtual bool isStageFinished(){
      return false;
  };
};