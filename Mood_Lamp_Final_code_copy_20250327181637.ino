//Date: 14/03/2025 - 16/03/2025 

//------------------------------------------Libraries------------------------------------------
#include <Adafruit_NeoPixel.h>            //version 1.12.4. for controlling RGB ring
#include <IRremote.h>                     //version 2.6.1. Go to tool -> manage library -> search for: IRremote -> install version: 2.6.1
#include <ir_Lego_PF_BitStreamEncoder.h>  //for encoding IR signals

//--------------Components pins and initial setup--------------

#define PIXEL_PIN 6  // Digital IO pin connected to the NeoPixels.
#define PIXEL_PIN_2 8  // Digital IO pin connected to the NeoPixels.

const int RECV_PIN = 7;  //reciver pin

IRrecv irrecv(RECV_PIN);
decode_results cmd;

#define PIXEL_COUNT 25  //NUMBER OF LEDS ON THE RGB RING

//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_2, NEO_GRB + NEO_KHZ800);  

//--------------defining avalible remote buttons (ELEGOO Remote)--------------
#define Button_0 0xFF6897
#define Button_1 0xFF30CF
#define Button_2 0xFF18E7
#define Button_3 0xFF7A85
#define Button_4 0xFF10EF
#define Button_5 0xFF38C7
#define Button_6 0xFF5AA5
#define Button_7 0xFF42BD
#define Button_8 0xFF4AB5
#define Button_9 0xFF52AD
#define Play_forward_button 0xFFC23D
#define Play_back_button 0xFF22DD
#define Down_button 0xFFE01F
#define Up_button 0xFF906F
#define VolUp_button 0xFF629D
#define VolDown_button 0xFFA857
#define Func_stop 0xFFE21D
//------------------------------------------SETUP------------------------------------------
void setup() {
  pinMode(PIXEL_PIN, OUTPUT);
  strip.setBrightness(50); 
  strip.begin();               //prepares to send data
  strip.show();                // Initialize all pixels to 'off' (show() toggles RGB ring)

  pinMode(PIXEL_PIN_2, OUTPUT);
  strip_2.setBrightness(50);   //LED brightness function, set all LED to max brightness,from 0 to 255
  strip_2.begin();
  strip_2.show();              // Initialize all pixels to 'off' (show() toggles RGB ring)

  randomSeed(analogRead(A0));  //seeding, as to ensure we get a random number, selecting colour on mood lamp start up
  Serial.begin(9600);          //make sure serial monitor is set to 9600

  irrecv.enableIRIn();
  irrecv.blink13(true);  //flash reciver when signle detected
}

//------------------Definging second RGB ring varibles as globa------------------
uint32_t static_colour_2 = strip_2.Color(random(0, 256), random(0, 256), random(0, 256));  // Retains value across calls
uint32_t Colour_2_1 = strip_2.Color(255, 0, 0);
uint32_t Colour_2_2 = strip_2.Color(0, 255, 0);
uint32_t Colour_2_3 = strip_2.Color(0, 0, 255);
uint32_t colours_2[] = { Colour_2_1, Colour_2_2, Colour_2_3 };

//------------------------------------------MAIN------------------------------------------
void loop() {
  uint32_t Colour_1 = strip.Color(255, 0, 0); // Red colour
  uint32_t Colour_2 = strip.Color(0, 255, 0); // Green colour
  uint32_t Colour_3 = strip.Color(0, 0, 255); // Blue colour
  uint32_t colours[] = { Colour_1, Colour_2, Colour_3 }; // Store colours in an array
  
  byte brightness = 55;     // Initializing RGB brightness, user can change this value
  int steps = 30;           // Initializing smooth transistion delay, user can change this value
  uint8_t wait = 500;       // Initializing delay between the RGB leds
  static_colour_2 = strip_2.Color(random(0, 256), random(0, 256), random(0, 256));  // Set random colour for strip 2
  uint32_t static_colour = strip.Color(random(0, 256), random(0, 256), random(0, 256));  // Set random colour for strip 1


  uint32_t start_up_colour = random(65000);  // Random startup colour value
  strip.fill(strip.ColorHSV(start_up_colour)); // Set the LED strip to random colour                        
  strip_2.fill(strip.ColorHSV(start_up_colour)); // Set the second strip to the same random colour   
  strip.show(); // Update the first strip with the colour
  strip_2.show(); // Update the second strip with the colour

  while (1) {  // Infinite loop to continuously check for button presses
    if (irrecv.decode(&cmd)) { // Check if an IR command has been received
      readRemote();  //Read IR reciver

      //--------------Static colour selection-------------- 
      if (cmd.value == Button_0) {  // When Button 0 is pressed
        Serial.println("Static colour selection");  // Output to serial monitor
        strip.setBrightness(brightness);  // Set the brightness of the first strip
        strip_2.setBrightness(brightness);  // Set the brightness of the second strip
        static_colour = colour_selection(static_colour);  // Get user-selected static colour
        delay(1000);  // Wait for 1 second
      }

      //--------------Dimming--------------
      if (cmd.value == Button_1) {  // when button one is pressed //let user change delay
        Serial.println("Dimming");  // Output to serial monitor
        brightness = dimming(static_colour, wait, brightness); // Adjust brightness
        delay(1000);  // Wait for 1 second
      }

      //--------------Multi colour--------------
      if (cmd.value == Button_2) {  // when button 2 is pressed
        Serial.println("Multi colour"); // Output to serial monitor
        multi_colour_illumination(colours, wait, brightness, steps); // Start multi-colour illumination
        delay(1000);  // Wait for 1 second
      }

      //--------------Timing mode-------------- // to let user select steps
      if (cmd.value == Button_5) {  // when button 5 is pressed 
        Serial.println("Timing mode"); // Output to serial monitor
        steps = timing_selection(steps); // Get user-selected timing steps
        delay(1000);  // Wait for 1 second
      }

      //--------------White light mode--------------
      if (cmd.value == Button_6) {  // when button 6 is pressed
        Serial.println("White light mode");  // Output to serial monitor
        white_light(brightness);  // Activate white light mode
        delay(1000);  // Wait for 1 second
      }
    }
  }
}

//------------timing mode------------
int timing_selection(int steps){
  while(true){
    if (irrecv.decode(&cmd)) {
      readRemote();
      if (cmd.value == Play_forward_button) {
        steps = steps + 25;  //increment of 25 
      }
      if (cmd.value == Play_back_button) {
        steps = steps - 25;  //decrement of 25 
      }
      if (cmd.value == Up_button) {
        steps = steps + 5;  //increment of 5 
      }
      if (cmd.value == Down_button) {
        steps = steps - 5;  //decrement of 5
      }
      if (cmd.value == VolUp_button) {
        steps = steps + 1; //increment of 1 
      }
      if (cmd.value == VolDown_button) {
        steps = steps - 1;  //decrement of 1
      }
    }
    Serial.println(steps); //check
  }
  return steps;
}

//------------multi colour illumination------------
void multi_colour_illumination(uint32_t colours[], uint8_t wait, byte brightness, int steps) {
  while (1) {  // Infinite loop to continuously check for button presses
    if (irrecv.decode(&cmd)) {  // Check if an IR command has been received
      readRemote();  // Read the IR command
      if (cmd.value == Func_stop) {  // If the stop button is pressed
        return;  // Exit the function
      }

      //--------------Defult RGB mode--------------
      if (cmd.value == Button_8) {  // If Button 8 is pressed
        RGB();  // Activate default RGB mode
      }

      //--------------Strobe light or flash mode--------------
      if (cmd.value == Button_2) {  // If Button 2 is pressed
        for(int c = 0; c <= 2; c++){  // Loop through the three colours
          colours[c] = colour_selection(colours[c]);  // Get user-selected colour
          Serial.println(colours[c]);  // Output the selected colour to serial monitor
        }
        Serial.println("got colours");  // Output to serial monitor
        while (true) {  // Infinite loop for continuous colour transition
          if (irrecv.decode(&cmd)) {  // Check if an IR command has been received
            readRemote();  // Read the IR command
            if (irrecv.decode(&cmd)) {  // Check again for any received IR command
              return;  // Exit the function if an IR command is detected
            }
            //--------------Strobe light-------------
            if (cmd.value == Button_3) {  // If Button 3 is pressed
              Serial.println("Strobe light");  // Output to serial monitor
              while (true) {  // Infinite loop for strobe light effect
                for (int i = 0; i <= 2; i++) {  // Loop through the three colours
                  if (irrecv.decode(&cmd)) {  // Check if an IR command has been received
                    return;  // Exit the function if an IR command is detected
                  }
                  int nextColour = (i + 1) % 3;  // Get the next colour in the sequence
                  fadeBetweenColours(colours[i], colours[nextColour], steps, brightness);  // Fade between colours
                }
              }
            }
            //--------------Flash mode--------------
            if (cmd.value == Button_4) {  // If Button 4 is pressed
              Serial.println("Flash mode");  // Output to serial monitor
              while (true) {  // Infinite loop for flash effect
                for (int i = 0; i <= 2; i++) {  // Loop through the three colours
                  if (irrecv.decode(&cmd)) {  // Check if an IR command has been received
                    return;  // Exit the function if an IR command is detected
                  }
                  int nextColour = (i + 1) % 3;  // Get the next colour in the sequence
                  fadeBetweenColours(colours[i], colours[nextColour], 15, brightness);  // Flash between colours
                }
              }
            }
          }
        }
      }
    }
  }
}

//------------Crossfade Between Multiple colours------------
void fadeBetweenColours(uint32_t colour1, uint32_t colour2, int steps, byte brightness) {
  // Extract RGB components of the first colour
  uint8_t r1 = (colour1 >> 16) & 0xFF;
  uint8_t g1 = (colour1 >> 8) & 0xFF;
  uint8_t b1 = colour1 & 0xFF;

  // Extract RGB components of the second colours
  uint8_t r2 = (colour2 >> 16) & 0xFF;
  uint8_t g2 = (colour2 >> 8) & 0xFF;
  uint8_t b2 = colour2 & 0xFF;
  
  strip.setBrightness(brightness);  // Set brightness for the first strip
  strip_2.setBrightness(brightness);  // Set brightness for the second strip

  // Smooth transition between the two colours
  for (int i = 0; i <= steps; i++) {
    // Calculate intermediate RGB values between the two colours
    uint8_t r = r1 + ((r2 - r1) * i / steps);
    uint8_t g = g1 + ((g2 - g1) * i / steps);
    uint8_t b = b1 + ((b2 - b1) * i / steps);

    uint32_t blendedColour = strip.Color(r, g, b);  // Combine the RGB values for the first strip
    uint32_t blendedColour_2 = strip_2.Color(r, g, b);  // Combine the RGB values for the second strip
    for (int j = 0; j < PIXEL_COUNT; j++) {  // Loop through each pixel in the strip
      strip.setPixelColor(j, blendedColour);  // Set the colour of each pixel
      strip_2.setPixelColor(j, blendedColour_2);  // Set the colour of each pixel on the second strip
    }
    strip.show();  // Update the first strip with the new colour
    strip_2.show();  // Update the second strip with the new colour
    delay(30);  // Adjust delay for smooth transition speed
  }
}

//------------single colour selection------------
uint32_t colour_selection(uint32_t static_colour) {
  byte red = 0; // Initialize red component of colour to 0 (no red)
  byte green = 0; // Initialize green component of colour to 0 (no green)
  byte blue = 0; // Initialize blue component of colour to 0 (no blue)
  uint16_t hue = 65000; // Initial hue value (not used in this function but might be part of future changes)
  int i = 0; // Index for preset colours
  uint32_t preset_colours[] = {
    strip.Color(255, 0, 0),   // Red
    strip.Color(0, 255, 0),   // Green
    strip.Color(0, 0, 255),   // Blue
    strip.Color(255, 255, 0), // Yellow
    strip.Color(255, 165, 0), // Orange
    strip.Color(128, 0, 128), // Purple
    strip.Color(255, 192, 203), // Pink
    strip.Color(0, 255, 255), // Cyan
    strip.Color(139, 69, 19), // Brown
    strip.Color(255, 255, 255) // White
  };
  uint32_t preset_colours_2[] = {
    strip_2.Color(255, 0, 0),   // Red
    strip_2.Color(0, 255, 0),   // Green
    strip_2.Color(0, 0, 255),   // Blue
    strip_2.Color(255, 255, 0), // Yellow
    strip_2.Color(255, 165, 0), // Orange
    strip_2.Color(128, 0, 128), // Purple
    strip_2.Color(255, 192, 203), // Pink
    strip_2.Color(0, 255, 255), // Cyan
    strip_2.Color(139, 69, 19), // Brown
    strip_2.Color(255, 255, 255) // White
  };
  while (1) {
    if (irrecv.decode(&cmd)) {
      readRemote();  //read IR reciver
      //------------edit red------------
      if (cmd.value == Func_stop) {
        break;
      }
      if (cmd.value == Button_1) {
        while (1) { // Loop for continuous red colour adjustment
          Serial.println("red selection");
          if (irrecv.decode(&cmd)) {  //edit red
            red = update_colour(red);
            strip.fill(strip.Color(red, green, blue));
            strip_2.fill(strip_2.Color(red, green, blue));
            Serial.println(red);  //check
            strip.show();         //hardware update of RGB light
            strip_2.show();
            if (cmd.value == Func_stop) {
              break;
            }
          }
        }
        static_colour_2 = strip_2.Color(red, green, blue);
        static_colour =strip.Color(red, green, blue);
      }
      //------------edit green------------
      if (cmd.value == Button_2) {
        while (1) { // Loop for continuous green colour adjustment
          Serial.println("green selection");
          if (irrecv.decode(&cmd)) {  //edit red
            green = update_colour(green);
            strip.fill(strip.Color(red, green, blue));
            strip_2.fill(strip_2.Color(red, green, blue));
            Serial.println(green);  //check
            strip.show();           //hardware update of RGB light
            strip_2.show();
            if (cmd.value == Func_stop) {
              break;
            }
          }
        }
        static_colour_2 = strip_2.Color(red, green, blue);
        static_colour=strip.Color(red, green, blue);
      }
      //------------edit blue------------
      if (cmd.value == Button_3) { // Loop for continuous blue colour adjustment
        while (1) { // Loop for continuous blue colour adjustment
          Serial.println("blue selection");
          if (irrecv.decode(&cmd)) {  //edit red
            blue = update_colour(blue);
            // Update first RGB Rings
            strip.fill(strip.Color(red, green, blue));
            strip_2.fill(strip.Color(red, green, blue));
            Serial.println(blue);  //check
            strip.show();          //hardware update of RGB light
            strip_2.show();
            if (cmd.value == Func_stop) {
              break;
            }
          }
        }
        static_colour_2 = strip_2.Color(red, green, blue);
        static_colour=strip.Color(red, green, blue);
      }
      //------------HSL colour------------
      if (cmd.value == Button_4) { //allowing user to choose colour from a default colour pallet
        Serial.println("hsl colour");
        while (1) {
          if (irrecv.decode(&cmd)) {  
            readRemote();
            
            if (cmd.value == VolUp_button) {
              i = i + 1; // next colour in preset
            }
            if (cmd.value == VolDown_button) {
              i = i - 1;  // previous colour in preset
            }
            Serial.println(i);  
            if(i == 10){
              i = 0;  // Loop back to the first colour
            }
            if(i == -1 ){
              i = 9;  // Loop back to the last colour
            }
            Serial.println(i);  
             // Set the current colour based on the selected index from preset colours
            static_colour = preset_colours[i]; //decrement of 1 
            static_colour_2 = preset_colours_2[i]; //decrement of 1 
            Serial.println(i);  
            strip.fill(static_colour); // Apply the selected colour to the first LED Ring
            strip_2.fill(static_colour_2); // Apply the selected colour to the second LED Ring
            Serial.println(static_colour);  //check
            strip_2.show();          //hardware update of RGB light
            strip.show();          //hardware update of RGB light
            if(cmd.value == Func_stop) {
              break;
            }
          }
        }        
      }         
    }
  }
  return static_colour;  // Initializing colour;
}

//-------------update current colour-------------
byte update_colour(byte colour) {
  Serial.println(colour);  //check
  readRemote();
  if (cmd.value == Play_forward_button) {
    colour = colour + 25;  //increment of 25 (10 steps from 0 to 255) 
  }
  if (cmd.value == Play_back_button) {
    colour = colour - 25;  //decrement of 25 
  }
  if (cmd.value == Up_button) {
    colour = colour + 5;  //increment of 5 (51 steps from 0 to 255) 
  }
  if (cmd.value == Down_button) {
    colour = colour - 5;  //decrement of 5
  }
  if (cmd.value == VolUp_button) {
    colour = colour + 1; //increment of 1 (255 steps from 0 to 255) 
  }
  if (cmd.value == VolDown_button) {
    colour = colour - 1;  //decrement of 1
  }
  return colour; //return updated current colour
}

//--------------Dimming mode--------------
byte dimming(uint32_t static_colour, uint8_t wait, byte brightness) {
  Serial.println(brightness);  //check
  while (1) {
    if (irrecv.decode(&cmd)) {
      readRemote();  //read IR reciver

      if (cmd.value == Play_forward_button) {  //increase RGB brightness by higher factor (10)
        brightness = brightness + 25;          //increment of 25 (10 steps from 0 to 255) was chosen as it can be repetative to change RGB brightness at lower increment
        Serial.println(brightness);            //check
        strip.setBrightness(brightness);       //update ring brightness
        strip.show();                          //hardware update of RGB light
        strip_2.setBrightness(brightness);       //update ring brightness
        strip_2.show();                          //hardware update of RGB light
      }

      if (cmd.value == Play_back_button) {  //deccrease RGB brightness by hugher factor (10)
        brightness = brightness - 25;       //increment of 25 (10 steps from 0 to 255) was chosen as it can be repetative to change RGB brightness at lower increment
        Serial.println(brightness);         //check
        strip.setBrightness(brightness);    //update ring brightness
        strip.show();                       //hardware update of RGB light
        strip_2.setBrightness(brightness);       //update ring brightness
        strip_2.show();                          //hardware update of RGB light
      }

      if (cmd.value == Up_button) {       //increase RGB brightness by small factor (5)
        brightness = brightness + 5;      //increment brightness by 5
        Serial.println(brightness);       //check
        strip.setBrightness(brightness);  //update ring brightness
        strip.show();                     //hardware update of RGB light
        strip_2.setBrightness(brightness);       //update ring brightness
        strip_2.show();                          //hardware update of RGB light
      }

      if (cmd.value == Down_button) {     //deccrease RGB brightness by samll factor (5)
        brightness = brightness - 5;      //decrement brightness by 5
        Serial.println(brightness);       //check
        strip.setBrightness(brightness);  //update ring brightness
        strip.show();                     //hardware update of RGB light
        strip_2.setBrightness(brightness);       //update ring brightness
        strip_2.show();                          //hardware update of RGB light
      }


      if (cmd.value == VolUp_button) {       //increase RGB brightness by small factor (5)
        brightness = brightness + 1;      //increment brightness by 5
        Serial.println(brightness);       //check
        strip.setBrightness(brightness);  //update ring brightness
        strip.show();                     //hardware update of RGB light
        strip_2.setBrightness(brightness);       //update ring brightness
        strip_2.show();                          //hardware update of RGB light
      }

      if (cmd.value == VolDown_button) {     //deccrease RGB brightness by samll factor (5)
        brightness = brightness - 1;      //decrement brightness by 5
        Serial.println(brightness);       //check
        strip.setBrightness(brightness);  //update ring brightness
        strip.show();                     //hardware update of RGB light
        strip_2.setBrightness(brightness);       //update ring brightness
        strip_2.show();                          //hardware update of RGB light
      }

      if (cmd.value == Func_stop) {  //when button one is presses again user can stop changing RGB brightness
        break;                      //to exit function
      }
    }
  }
  return brightness;
}

//--------------used to read IR reciver--------------
void readRemote() {                //must be called in if statment
  Serial.println(cmd.value, HEX);  //prints signal from remote decoded to HEX
  delay(1500);                     //so we dont read reciver signal to quick, which can cause signal corruption
  irrecv.resume();                 //read reciver for signal
}

//--------------RGB mode--------------
void RGB() {
  uint32_t hue; // Declare variable for storing hue value in HSV colour space
  while(true){ // Infinite loop to continuously cycle through colours
    for (hue = 0; hue <= 65535; hue += 1000) { // Loop through hue values (from 0 to 65535, incrementing by 1000)
      for (uint16_t i = 0; i <= PIXEL_COUNT; i++) { // Loop through each pixel on the strip
        strip.setPixelColor(i, strip.ColorHSV(hue));  // Set the colour of each pixel based on the current hue value using HSV colour model
        strip_2.setPixelColor(i, strip_2.ColorHSV(hue)); // Same for the second strip
        if (irrecv.decode(&cmd)) {  //since neopixel does not give enough time to read IR remote for valid signal will just exit on any ir signal does not realy matter (if it works it works...)
          return;                   //exit to main loop
        }
        strip.show(); // Update the first LED strip with the new colour
        strip_2.show(); // Update the second LED strip with the new colour
        Serial.println(hue); //check
        delay(10);  // Small delay to slow down the colour transition (10 ms)
      }
    }
  }
}

//--------------White light mode--------------
void white_light(byte brightness) {
  // colour Name	RGB Values (R, G, B)	Description
  // Deep Orange	(255, 140, 0)	Rich and saturated
  // Vibrant Orange	(255, 120, 10)	More contrasty
  // Golden Orange	(255, 180, 50)	Warmer tone
  // Fiery Orange	(255, 90, 10)	Very strong reddish-orange
  // Burnt Orange	(204, 85, 0)	Darker orange

  while(true){
    if (irrecv.decode(&cmd)) {  //since neopixel does not give enough time to read IR remote for valid signal will just exit on any ir signal does not realy matter (if it works it works...)
    return;                   //exit to main loop
    }  
    fadeBetweenColours(strip.Color(255, 120, 0), strip.Color(0, 0, 255), 100, brightness); //transistiion from orange to blue
    fadeBetweenColours(strip.Color(0, 0, 255), strip.Color(255, 120, 0), 100, brightness); //transistiion from blue to orange
  }
}

/*---------------------------------------------NOTES---------------------------------------------------
  -------------------------------------USEFUL DOCUMENTATION--------------------------------------------
  https://forum.arduino.cc/t/how-you-could-control-the-led-rgb-strip-part-1/344777
  https://wiki.seeedstudio.com/Grove-LED_ring/
  https://www.youtube.com/watch?v=EBznuu88FDQ
  https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html#a310844b3e5580056edf52ce3268d8084
  https://docs.arduino.cc/language-reference/en/variables/data-types/unsignedInt/
  strip.color(R,G,B), used to select colour, 0 - 255 per LED
  strip.show(), used to toggle between ON/OFF current RGB LED colour
  ------------------------------------PROBLEMS AND SOLUTIONS-------------------------------------------
  PROBLEM 1:
  READING IR RECIVER AS RGB COLOUR IS SHOWN, THE NEOPIXEL LIBRARY DISSABLES ALL OTHER INTERRUPT MAKING USE OF INTERRUPTS NOT POSSIBLE
  MILLI, ONLY ALLOWS FOR MULTITASKING AND THUS DOES NOT INTERRUPT. 
  SOLUTION TO PROBLEM 1:
  LINK: https://forum.arduino.cc/t/how-to-exit-immediately-from-a-set-of-loops/45431 - post 3
  I thought intead of taking a valid signal why not just take any ir signal even corrupted
  as to then exit function. Furthermore this was done by 'return'ing nothing as to exit function 
  mood lamp mode. 
  -
  PROBLEM 2:

  SOLUTION TO PROBLEM 2:
  LINK: 
*/
//add option 7 so user can restart by exiting main loop
//add rgb optiion
//add rainbow option

// strip.setPixelColor(i, (0, 63, 0));  //1/4 bright green
// strip.setPixelColor(i, (255, 0, 0));  //full-bright red
// strip.setPixelColor(i, (0, 255, 255)); //full-bright cyan
// strip.setPixelColor(i, (127, 127, 0)); //half-bright yellow
// strip.setPixelColor(i, (255, 192, 255)); //orange
// strip.setPixelColor(i, (63, 63, 63)); //1/4-bright white