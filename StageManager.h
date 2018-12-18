#include "IntEditingStage.h"
#include "MenuStage.h"
#include "DefaultStage.h"

class StageManager: Stage{
  bool isInMenu = false;
  long menuTimeoutStart = 0;
  MenuStage menu;
  DefaultStage defaultStage;
  public:

  StageManager(){
    menuTimeoutStart = millis();
  }
  
  virtual void render(Adafruit_SSD1306 * display){
    // if(millis() - menuTimeoutStart > 10000){
    //   isInMenu = false;
    // }

    display->clearDisplay();
    if(isInMenu){
      menu.render(display); 
      if(menu.isStageFinished()){
        isInMenu = false;
        defaultStage.resetTimer();
      }
    }else{
      defaultStage.render(display);
    }
    display->display();
  };

  virtual void encoderRotated(int deltaSteps){
    if(!isInMenu){
      isInMenu = true;
      menu.setSelected(0);
      menu.shouldExit = false;
    }else{
      menu.encoderRotated(deltaSteps);
    }
  };

  virtual void encoderButtonPressed(){
    if(isInMenu){
      menu.encoderButtonPressed();
    }else{
      defaultStage.encoderButtonPressed();
    }
  };

  virtual bool isStageFinished(){
    return false;
  }
};
