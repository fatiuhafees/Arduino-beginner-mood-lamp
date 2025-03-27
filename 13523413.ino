#include <Adafruit_NeoPixel.h>

#define LED_PIN    6   // LED strip connected to pin 6
#define LED_COUNT  30  // Number of LEDs in the strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'

  // Define initial colors
  uint32_t Colour_1 = strip.Color(255, 0, 0);  // Red
  uint32_t Colour_2 = strip.Color(0, 255, 0);  // Green
  uint32_t Colour_3 = strip.Color(0, 0, 255);  // Blue

  uint32_t colours[] = {Colour_1, Colour_2, Colour_3};
  for(int i = 0; i < 255; i++){
    // Extract the red component and modify it
    uint8_t red = (colours[0] >> 16) & 0xFF;  // Extract red for green shift by 8 and for blue not at all (RRRRRRRR GGGGGGGG BBBBBBBB)
    red = red + i;  // Modify red value (example: double it) 

    // Ensure red doesn't exceed 255
    if (red > 255) red = 255;

    // update current color with new red value
    colours[0] = strip.Color(red, (colours[0] >> 8) & 0xFF, colours[0] & 0xFF);

    // Set the first LED to the modified color
    strip.fill(colours[0]);
    strip.show();
    delay(500);
  }
}

void loop() {
  // Nothing in loop
}
