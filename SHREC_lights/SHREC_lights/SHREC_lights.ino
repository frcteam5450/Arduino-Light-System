#include <Adafruit_NeoPixel.h> //for controlling the LEDs
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_ONE 6 //these numbers correspond to the led strip ports
#define PIN_TWO 5
#define PIN_THREE 3


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(60, PIN_ONE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(60, PIN_TWO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(60, PIN_THREE, NEO_GRB + NEO_KHZ800);

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

  pinMode(0, INPUT); //sets the dio pin values that connect the Roborio with the Arduino
  pinMode(1, INPUT);
  pinMode(2, INPUT);

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
  state = 0; //reset
  in0 = (digitalRead(0) == HIGH);
  in1 = (digitalRead(1) == HIGH);
  in2 = (digitalRead(2) == HIGH);
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
  //check if the light is on
  // Some example procedures showing how to display to the pixels:
  if (state == 0) { // Robot is off
    colorAll(strip1.Color(0, 0, 0)); //all leds turn off

  } else if (state == 1) { // Mechanum drive is enabled
    colorFlash(strip1.Color(15, 0, 15), strip1.Color(0, 0, 0), 1000); //all leds flash magenta

  } else if (state == 2) { // Traction drive is enabled
    colorFlash(strip1.Color(0, 30, 0), strip1.Color(0, 0, 0), 1000); //all leds flash green

  } else if (state == 3) { // robot is currently shooting fuel
    theaterChase(strip1.Color(10, 10, 0), 500); //theater chase yellow

  } else if (state == 4) { // robot is aiming
    colorFlash(strip1.Color(20, 20, 0), strip1.Color(0, 0, 0), 1000); //flashing yellow

  } else if (state == 5) { // robot is ready for gear
    colorDance(strip1.Color(0, 50, 0), 500); //leds dance green

  } else if (state == 6) { // robot is climbing
    colorWipe(strip1.Color(0, 20, 10), 50); //wipe turquoise
    colorWipe(strip1.Color(10, 10, 10), 50); //wipe white
    colorWipeReverse(strip1.Color(0, 20, 10), 50); //reverse wipe turquoise
    colorWipeReverse(strip1.Color(10, 10, 10), 50); //reverse wipe white

  } else if (state == 5450) { //We won the match! Victory!
    rainbowCycle(20); //rainbow! :D


  }
}

// Fill the dots one after the other with a color


void colorWipe(uint32_t c, uint8_t wait) {

  for (uint16_t i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, c);
    strip2.setPixelColor(i, c);
    strip3.setPixelColor(i, c);
    strip1.show();
    strip2.show();
    strip3.show();
    delay(wait);
  }
}
// Fill the dots in reverse
void colorWipeReverse(uint32_t c, uint8_t wait) {
  for (uint16_t i = strip1.numPixels(); i > 0; i--) {
    strip1.setPixelColor(i, c);
    strip2.setPixelColor(i, c);
    strip3.setPixelColor(i, c);
    strip1.show();
    strip2.show();
    strip3.show();
    delay(wait);
  }
}
void colorFlash(uint32_t c1, uint32_t c2, uint8_t wait) {
  for (uint16_t i = strip1.numPixels(); i > 0; i--) {
    strip1.setPixelColor(i, c1);
    strip2.setPixelColor(i, c1);
    strip3.setPixelColor(i, c1);
  }
  strip1.show();
  strip2.show();
  strip3.show();
  delay(wait);
  for (uint16_t i = strip1.numPixels(); i > 0; i--) {
    strip1.setPixelColor(i, c2);
    strip2.setPixelColor(i, c2);
    strip3.setPixelColor(i, c2);
  }
  strip1.show();
  strip2.show();
  strip3.show();
  delay(wait);
}
void colorAll(uint32_t c) {
  for (uint16_t i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, c);
    strip2.setPixelColor(i, c);
    strip3.setPixelColor(i, c);
  }
  strip1.show();
  strip2.show();
  strip3.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel((i + j) & 255));
      strip2.setPixelColor(i, Wheel((i + j) & 255));
      strip3.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip1.show();
    strip2.show();
    strip3.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
      strip2.setPixelColor(i, Wheel(((i * 256 / strip2.numPixels()) + j) & 255));
      strip3.setPixelColor(i, Wheel(((i * 256 / strip3.numPixels()) + j) & 255));
    }
    strip1.show();
    strip2.show();
    strip3.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int q = 0; q < 3; q++) {
    for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
      strip1.setPixelColor(i + q, c);  //turn every third pixel on
      strip2.setPixelColor(i + q, c);
      strip3.setPixelColor(i + q, c);
    }
    strip1.show();
    strip2.show();
    strip3.show();

    delay(wait);

    for (uint16_t i = 0; i < strip1.numPixels(); i += 3) {
      strip1.setPixelColor(i + q, 0);      //turn every third pixel off
      strip2.setPixelColor(i + q, 0);
      strip3.setPixelColor(i + q, 0);
    }
  }
}

void colorDance(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 2; j++) { //do 2 cycles of dancing
    for (int q = 0; q < 2; q++) {
      for (uint16_t i = 0; i < strip1.numPixels(); i += 2) {
        strip1.setPixelColor(i + q, c);  //turn every second pixel on
        strip2.setPixelColor(i + q, c);
        strip3.setPixelColor(i + q, c);
      }
      strip1.show();
      strip2.show();
      strip3.show();

      delay(wait);

      for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
        strip1.setPixelColor(i + q, 0);      //turn every third pixel off
        strip2.setPixelColor(i + q, 0);
        strip3.setPixelColor(i + q, 0);
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip1.numPixels(); i += 3) {
        strip1.setPixelColor(i + q, Wheel((i + j) % 255));
        strip2.setPixelColor(i + q, Wheel((i + j) % 255));
        strip3.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
      }
      strip1.show();
      strip2.show();
      strip3.show();

      delay(wait);

      for (uint16_t i = 0; i < strip1.numPixels(); i += 3) {
        strip1.setPixelColor(i + q, 0);      //turn every third pixel off
        strip2.setPixelColor(i + q, 0);
        strip3.setPixelColor(i + q, 0);
      }
    }
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
