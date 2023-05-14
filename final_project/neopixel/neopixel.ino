#include <Adafruit_NeoPixel.h>

#define LED_PIN   9   //The pin connected to the NeoPixels.
#define LED_COUNT 50//The number of NeoPixels attached.
#define DELAY_VAL 100


//Declare our NeoPixel strip object:
//Arg 1: # of pixels; Arg 2: Arduino pin #; Arg 3: pixel type flags, add together as needed

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); //Initialize all pixels to off.
  strip.setBrightness(12);
}

void loop() {
  strip.clear();
  for(int i=0; i<LED_COUNT; i++) {
     strip.setPixelColor(i, min(50+10*i,255),min(255-20*i,255),min(20+15*i,255));
     strip.show();
     delay(DELAY_VAL);           
  }
            
            
//  for(int i=0; i<LED_COUNT; i++) {
//    strip.rainbow(65535-(i*6553),1,255,255,true);
//    strip.show();
//    delay(100);
//  }
}
