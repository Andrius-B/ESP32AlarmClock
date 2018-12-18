#include "Stage.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class IntEditingStage: Stage{
  private:
  int value = 0;
  int maxValue = INT_MIN;
  int minValue = INT_MAX;
  int minIncrements = 1;
  int target_prop = 0;
  String title;
  public:
    bool shouldExit = false;
    int getValue(){
      return value;  
    }

    void setValue(int newValue){
      value = newValue;
    }

    void setTitle(String newTitle){
        title = newTitle;
    }

    String getTitle(){
      return title;  
    }

    void setTarget(int target){
      target_prop = target;
    }

    int getTarget(){
      return target_prop;
    }

    void setMaxValue(int newMaxValue){
      this->maxValue = newMaxValue;
    }

    void setMinValue(int newMinValue){
      this->minValue = newMinValue;
    }

    void setMinIncrement(int minIncrement){
      this->minIncrements = minIncrement;
    }
    
    void encoderRotated(int deltaSteps){
      value += deltaSteps*minIncrements;
      if(maxValue != INT_MIN && minValue != INT_MAX){
        if(value > maxValue){
          value = minValue;
        }else if(value < minValue){
          value = maxValue;
        }
      }
    }
    
    virtual void render(Adafruit_SSD1306 * display){
        display->setTextSize(2);      // Normal 1:1 pixel scale
        display->setTextColor(WHITE); // Draw white text
        display->setCursor(5,8);
        display->println(title + ":" + String(value));
    }

    virtual void encoderButtonPressed(){
      shouldExit = true;
    };

    virtual bool isStageFinished(){
      return shouldExit;
  }
};
