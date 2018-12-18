#include "MenuRow.h"
#include "AlarmTime.h"

class MenuStage: Stage{
    constexpr static int NumRows = 3;
    MenuRow rows[NumRows];
    int currentSelection;
    bool isInPropEditStage = false;
    IntEditingStage propEditStage;
  public:
    bool shouldExit = false;
  MenuStage(){
    setRows();
    setSelected(0);
  }
  
    void setRows(){
        rows[0].title = "Hour";
        rows[0].content = String(alarmState.hour);
        rows[0].height = 10;

        rows[1].title = "Minute";
        rows[1].content = String(alarmState.minute);
        rows[1].height = 10;

        rows[2].title = "Exit";
        rows[2].content = "";
        rows[2].height = 10;
    }

    void setSelected(int index){
        rows[currentSelection].selected = false;
        if(index < 0){
            index = NumRows - 1;
        }else if(index >= NumRows){
            index = 0;
        }
        currentSelection = index;
        rows[index].selected = true;
    }

    int target = 0;
    unsigned int heightOffset;
  virtual void render(Adafruit_SSD1306 * display){
      if(isInPropEditStage){
          propEditStage.render(display);
          if(propEditStage.isStageFinished()){
              isInPropEditStage = false;
              rows[0].content = String(alarmState.hour);
              rows[1].content = String(alarmState.minute);
              if(propEditStage.getTarget() == 0){
                alarmState.hour = propEditStage.getValue();
              }else if(propEditStage.getTarget() == 1){
                  alarmState.minute = propEditStage.getValue();
              }
          }
      }
      else {
        setRows();
        heightOffset = 0;
        for(int i = 0; i < NumRows; i++){
            rows[i].render(display, heightOffset);
            heightOffset += rows[i].height;
        }  
      }
  };

  virtual void encoderRotated(int deltaSteps){
        if(isInPropEditStage){
            propEditStage.encoderRotated(deltaSteps);
        }else{
            setSelected(currentSelection + deltaSteps);
        }
  };

  virtual void encoderButtonPressed(){
      if(isInPropEditStage){
          propEditStage.encoderButtonPressed();
      }else{
          Serial.println("Entring prop edit!");
        isInPropEditStage = true;
        propEditStage.shouldExit = false;
        propEditStage.setTitle(rows[currentSelection].title);
        propEditStage.setTarget(currentSelection);
        if(currentSelection == 0){
            propEditStage.setMaxValue(23);
            propEditStage.setMinValue(0);
            propEditStage.setMinIncrement(1);
            propEditStage.setValue(alarmState.hour);
        }else if(currentSelection == 1){
            propEditStage.setMaxValue(59);
            propEditStage.setMinValue(0);
            propEditStage.setMinIncrement(1);
            propEditStage.setValue(alarmState.minute);
        }else if(currentSelection == 2){
            isInPropEditStage = false;
            shouldExit = true;
        }
    }
  };

  virtual bool isStageFinished(){
      return shouldExit;
  }
};