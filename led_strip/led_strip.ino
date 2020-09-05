#include <FastLED.h>

#define NUM_LEDS 296
#define DATA_PIN 5
#define BUTTON_PIN 2

CRGB leds[NUM_LEDS];
byte colorBuffer[1];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GBR>(leds, NUM_LEDS);
  while(!Serial) {;}
  Serial.write(1);
}

void cycle_forward() {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i-1];
  }
}

void cycle_backward() {
  for (int i = 0; i < NUM_LEDS-1; i++) {
    leds[i] = leds[i+1];
  }
}

void loop() {
  if(Serial.available() >= 3) {
    leds[NUM_LEDS-1].b = (byte)Serial.read();
    leds[NUM_LEDS-1].g = (byte)Serial.read();
    leds[NUM_LEDS-1].r = (byte)Serial.read();

    Serial.write(1);
    
    cycle_backward();
    
    FastLED.show();
  }
}
