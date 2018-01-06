#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Pin NeoPixels are plugged into
#define PIN 6

//Number of LEDs on NeoPixel Strand
#define NUM_LEDS 3

//pixel strand definition
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int roboRio1 = 2;

void setup() {
  // put your setup code here, to run once:
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pinMode(roboRio1 , INPUT);
}

void loop() {
  //reads value from roborio
  int robotState1 = digitalRead(roboRio1);

  //checks state of robot, if high, changes neopixel color
  if (robotState1 == HIGH) {

    //sets pixels to red
    for (int pixelVal = 0; pixelVal < NUM_LEDS; pixelVal++) {
      pixels.setPixelColor(pixelVal, pixels.Color(50 , 0 , 0));
    }
    
  }
  //changes color if RobotState1 isn't high
  else {

    //sets pixels to green
    for (int pixelVal = 0; pixelVal < NUM_LEDS; pixelVal++) {
      pixels.setPixelColor(pixelVal, pixels.Color(0 , 50 , 0));
    }
  } 

//displays neopixels
pixels.show();
}
