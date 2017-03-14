#include <Adafruit_NeoPixel.h> //for controlling the LEDs
#ifdef __AVR__
#include <avr/power.h>
#endif
/*
 * original values
 * 6
 * 5
 * 3
 */
#define PIN_ONE 9 // testing led//these numbers correspond to the led strip ports
#define PIN_TWO 10 //strip
#define PIN_THREE 11 //not connected


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(1, PIN_ONE, NEO_GRB + NEO_KHZ800); //default 60 leds
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(59, PIN_TWO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(60, PIN_THREE, NEO_GRB + NEO_KHZ800);
strip1.numPixels
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  Serial.begin(9600);

/*
 * previous values
 * 0
 * 1
 * 2
 */
  pinMode(2, INPUT); //sets the dio pin values that connect the Roborio with the Arduino
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show();
  strip3.show();
}
//declaring variables
int state; //this will determine what light sequence we will use
bool in0; //these determine the state
bool in1;
bool in2;

void loop() {
  state = 0; //reset the state each time
  in0 = digitalRead(3);
  in1 = digitalRead(4);
  in2 = digitalRead(5);
  if (!in0 && !in1 && !in2) { //all pins are off 000
    state = 0; //off

  } else if (in0 && !in1 && !in2) { //pin 1 is on, pins 2 and 3 are off 100
    state = 1; //mechanum

  } else if (!in0 && in1 && !in2) { //pin 2 is on, pins 1 and 3 are off 010
    state = 2; //traction

  } else if (!in0 && !in1 && in2) { //pin 3 is on, pins 1 and 2 are off 001
    state = 3; //shooting

  } else if (in0 && in1 && !in2) { //pins 1 and 2 are on, pin 3 is off 110
    state = 4; //aiming

  } else if (in0 && !in1 && in2) { //pins 1 and 3 are on, pin 2 is off 101
    state = 5; //waiting for gear

  } else if (!in0 && in1 && in2) { //pins 2 and 3 are on, pin 1 is off 011
    state = 6; //climbing

  } else if (in0 && in1 && in2) { //all pins are on 111
    state = 5450; //victory

  }
  
  /*
   * This determines what color patterns
   * will be played depending on the current circumstances
   */
  if (state == 0) { // Robot is off
    colorAll(strip1.Color(0,0,0);
    
  }else if(state == 1){ // Mechanum drive is enabled
    colorFlash(strip1.Color(25,0,25),strip1.Color(0,0,0),500);
    
  }else if(state == 2){ // Traction drive is enabled
    colorFlash(strip1.Color(0,50,0),strip1.Color(0,0,0),500);
    
  }else if(state == 3){ // robot is currently shooting fuel
    colorDance(strip1.Color(25,25,0),250);
    
  }else if(state == 4){ // robot is aiming
    colorFlash(strip1.Color(25,25,0),strip1.Color(0,0,0),500);
    
  }else if(state == 5){ // robot is ready for gear
    colorDance(strip1.Color(0,50,0),500);
    
  }else if(state == 6){ // robot is climbing
    colorFlash(strip1.Color(0,25,50),strip1.Color(50,25,0),500);
    
  }else if(state == 5450){ //We won the match! Victory!
    rainbowCycle(1);
    
  }
}


/*
 *This function flashes all leds two different colors
 *with the parameter "wait" as the delay in milliseconds
 *in between color changes. It first colors all leds
 *the color value of "c1" then waits before turning all
 *leds to the color value of "c2" and waits again
 */
void colorFlash(uint32_t c1, uint32_t c2, uint8_t wait) {
  for (uint16_t i = strip1.numPixels(); i > 0; i--) {
    strip1.setPixelColor(i, c1);
  }
  for (uint16_t i = strip2.numPixels(); i > 0; i--) {
    strip2.setPixelColor(i, c1);
  }
  for (uint16_t i = strip3.numPixels(); i > 0; i--) {
    strip3.setPixelColor(i, c1);
  }
  strip1.show();
  strip2.show();
  strip3.show();
  delay(wait);
  for (uint16_t i = strip1.numPixels(); i > 0; i--) {
    strip1.setPixelColor(i, c2);
  }
  for (uint16_t i = strip2.numPixels(); i > 0; i--) {
    strip2.setPixelColor(i, c2);
  }
  for (uint16_t i = strip3.numPixels(); i > 0; i--) {
    strip3.setPixelColor(i, c2);
  }
  strip1.show();
  strip2.show();
  strip3.show();
  delay(wait);
}
/*
 * This function sets all leds in the strips to the parameter "c"
 * which is the color value
 */
void colorAll(uint32_t c) {
  for (uint16_t i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, c);
  }
  for (uint16_t i = 0; i < strip2.numPixels(); i++) {
    strip2.setPixelColor(i, c);
  }
  for (uint16_t i = 0; i < strip2.numPixels(); i++) {
    strip2.setPixelColor(i, c);
  }
  strip1.show();
  strip2.show();
  strip3.show();
}

/*
 * This function basically cycles through the rainbow on the LEDs
 * and waits between changing colors based on the parameter "wait"
 */
void rainbowCycle(uint8_t wait){
  uint16_t i, j;

  for (j = 0; j < 256; j++) { 
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
    }
    for (i = 0; i < strip2.numPixels(); i++) {
      strip2.setPixelColor(i, Wheel(((i * 256 / strip2.numPixels()) + j) & 255));
    }
    for (i = 0; i < strip3.numPixels(); i++) {
      strip3.setPixelColor(i, Wheel(((i * 256 / strip3.numPixels()) + j) & 255));
    }
    strip1.show();
    strip2.show();
    strip3.show();
    delay(wait);
  }
}

/*
 * This function colors every other LED the color value of "c" and waits
 * the parameter "wait" in milliseconds before coloring those LEDs off
 * and colors the next set of LEDs
 */
void colorDanceStrip(uint32_t c, uint8_t wait) {
  for (int q = 0; q < 2; q++) {
    for (uint16_t i = 0; i < strip1.numPixels(); i += 2) {
      strip1.setPixelColor(i + q, c);  //turn every second pixel on
    }
      for (uint16_t i = 0; i < strip2.numPixels(); i += 2) {
        strip2.setPixelColor(i + q, c);
      }
      for (uint16_t i = 0; i < strip3.numPixels(); i += 2) {
        strip3.setPixelColor(i + q, c);
      }
      strip1.show();
      strip2.show();
      strip3.show();

      delay(wait);

      for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
        strip1.setPixelColor(i + q, 0);      //turn every second pixel off
      }
      for (uint16_t i = 0; i < strip2.numPixels(); i = i + 3) {
        strip2.setPixelColor(i + q, 0);
      }
      for (uint16_t i = 0; i < strip3.numPixels(); i = i + 3) {
        strip3.setPixelColor(i + q, 0);
      }
      strip1.show();
      strip2.show();
      strip3.show();

      delay(wait);
    }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
