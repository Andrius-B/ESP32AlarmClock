
/*
   This sketch shows an example of sending a reading to data.sparkfun.com once per day.
   It uses the Sparkfun testing stream so the only customizing required is the WiFi SSID and password.
   The Harringay Maker Space
   License: Apache License v2
*/

#include <ESP32Encoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "StageManager.h"
#include "AlarmTime.h"
#include "AlarmManager.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



char *ssid      = "UHHHH";               // Set you WiFi SSID
char *password  = "WIFI HERE";               // Set you WiFi password


ESP32Encoder encoder;


StageManager sm;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
   display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0,0);
  display.print("Setting up Wifi ");
  display.display();
  encoder.attachHalfQuad(18, 19);

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    display.print(".");
    display.display();
    delay(500);
  }
  display.clearDisplay();
  Serial.println("WiFi connected");
  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);
  pinMode(5, PULLUP);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  display.setTextSize(1);

  alarmState.hour = 9;
  alarmState.minute = 0;
}

bool alarmSet = false;

void loop() {
  // first parameter: Time zone in floating point (for India);
  // second parameter: 1 for European summer time; 2 for US daylight saving time (contributed by viewwer, not tested by me)
  
  // check dateTime.valid before using the returned time
  // Use "setSendInterval" or "setRecvTimeout" if required
  // display.setCursor(0,0);
  // display.clearDisplay();
  // display.println("Time:"+String(millis()));
  // display.display();
  // Serial.println("Display shown!");
  
  readEncoder();
  readEncoderButton();
  sm.render(&display);

  alarmManager.checkTime();
  if(alarmManager.shouldAlarmRing()){
    Serial.println("Ringing alarm!");
    digitalWrite(23, HIGH);
    alarmManager.setAlarmRung(true);
  }
}

bool buttonPressed = false;
bool buttonActivated = false;
long buttonPressedOnMillis = 0;
void readEncoderButton(){
  if(digitalRead(5) == HIGH){
    buttonPressed = false;
    buttonActivated = false;
  }

  if(digitalRead(5) == LOW && !buttonPressed){
    buttonPressedOnMillis = millis();
    buttonPressed = true;
  }

  if(buttonPressed && digitalRead(5) == LOW &&
      millis()-buttonPressedOnMillis > 5 &&  // simple debounce
      !buttonActivated){
    buttonActivated = true;
    onButtonPressed();
  }
}

void onButtonPressed(){
  sm.encoderButtonPressed();
}

void readEncoder(){
    if(encoder.getCount() >= 2 || encoder.getCount() <= -2){
      encoderValueChanged((int)encoder.getCount()/2);
      encoder.setCount(0);  
    }
}

void encoderValueChanged(int32_t deltaSteps){
  sm.encoderRotated(deltaSteps);
}
