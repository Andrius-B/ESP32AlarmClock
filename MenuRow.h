#include "Arduino.h"
#include <Adafruit_SSD1306.h>

class MenuRow{
  public:
    String title;
    String content;
    unsigned int height;
    unsigned int targetStageId;
    bool selected = false;

    void setSelected(bool active){
      this->selected = active;  
    }

    bool isSelected(){
      return this->selected;  
    }

    
    void render(Adafruit_SSD1306 * display, unsigned int verticalOffset){
      display->setTextSize(1);
      if(selected){
        display->setTextColor(BLACK);
        display->fillRect(0, verticalOffset, 128, height, WHITE);
      }else{
        display->setTextColor(WHITE); 
      }
        display->setCursor(4, verticalOffset+1);
        if(content != ""){
          display->println(title + ":" + content);
        }else{
          display->println(title);
        }
    }
};
