#include <WS2812FX.h>

//Nice colors
// blue 0x007BFF

unsigned long colors[] { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF, 0xFFFFFF };
int colorsLength = 7;


int currentColorIndex = 0;

//Circle = 32
//Stick = 17
#define LED_COUNT 32

#define LED_PIN 6

#define BTNTHRESHOLD_MS 100

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

bool lastClickState = false;
bool lastClickStateColor = false;

unsigned long last_btn = 0;
unsigned long now = 0;

void setup() {
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(colors[currentColorIndex]);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
  
  Serial.begin(9600);
}

void loop() {
  now = millis();
  
  if(now - last_btn > BTNTHRESHOLD_MS) {
    digitalWrite(13, LOW);
        
    //Animation Button
    bool click = !digitalRead(8);
    if(click != lastClickState){
      if(click){
        digitalWrite(13, HIGH);
        ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
      }else{
        last_btn = now;
      }
      lastClickState = click;
    }
    
    //Color Button
    bool clickColor = !digitalRead(9);
    if(clickColor != lastClickStateColor){
      if(clickColor){
        digitalWrite(13, HIGH);
        
        //Next Color
        currentColorIndex++;
        if(currentColorIndex >= colorsLength)
          currentColorIndex = 0;
        
        //Apply Color
        ws2812fx.setColor(colors[currentColorIndex]);
        
        Serial.print("Color Index: ");
        Serial.print(currentColorIndex);
        Serial.print(". Color Value: ");
        Serial.println(colors[currentColorIndex]);
      }else{
        last_btn = now;
      }
      lastClickStateColor = clickColor;
    }
    
  }

  ws2812fx.service();
}
