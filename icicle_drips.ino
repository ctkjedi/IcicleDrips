/* Icicle Drips

   By: Christopher Kirkman.

   Date: December-ish, 2019

   This sketch creates icicle drips down multiple strips.
   Falling drops increase in speed as they age, simulating gravity (poorly)

*/

#include <FastLED.h>

//The usual FastLED setup stuff
#define COLOR_ORDER RGB
#define CHIPSET     WS2811
#define NUM_LEDS    300
#define NUM_LEDS_PER_STRIP  60
#define BRIGHTNESS  50
#define FRAMES_PER_SECOND 30

//how many strips
int stripCount = 5;

//sets base speed that increases with age
float currSpeed = .05;

//how many drips total at any given time
int totalDrips = 10;


CRGB leds[NUM_LEDS];

//Custom class creates Drips object and stores position, age and which strip is resides on
class Drips {
    float stringPos; //positon on the strip
    int age;        //how old drip is to speed up
    int stripNumber;  //which strip the drip is on



  public:
    Drips(float yPos, int howOld, int whichStrip) {
      stringPos = yPos;
      age = howOld;
      stripNumber = whichStrip;
    }

    void updateDrip() {
      //if the current position is less than the last pixel on the strip, increase the position by
      //base speed multiplied by how old it is. Set that pixel's new color and increase it's age
      if (round(stringPos) < (NUM_LEDS_PER_STRIP * (stripNumber + 1))) {
        float newPos = stringPos + (currSpeed * age);
        stringPos = newPos;
        leds[round(newPos)] = CHSV(255, 0, random(128, 255));
        age++;
      } else {
        //otherwise run function to choose a new strip and reset it back to 0 position
        resetDrip();
      }

    }

    //Choose a new strip, set its position to the top of said strip and reset its age
    void resetDrip() {
      int sN = random(0, stripCount);
      age = 0;
      stripNumber = sN;
      stringPos = (sN * NUM_LEDS_PER_STRIP);
    }

    //A function to randomize drips across all strips
    void randomDrip() {
      int sN = random(0, stripCount);
      stripNumber = sN;
      stringPos = random(sN * NUM_LEDS_PER_STRIP, ( (sN + 1) * (NUM_LEDS_PER_STRIP) ) - 1);
      age = 0;
    }

};

//Array of 10 drips - would love to do this dynamically,
//looping through to create X amount based on totalDrips var
Drips drip[] = {
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0)
};


void setup() {
  // put your setup code here, to run once:
  delay(3000); // sanity delay

  //randomize all them drips
  for (int i = 0; i < totalDrips; i++) {
    drip[i].randomDrip();
  }

  //FastLED initializers
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.addLeds<WS2812, 21, COLOR_ORDER>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 19, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 17, COLOR_ORDER>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 14, COLOR_ORDER>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 13, COLOR_ORDER>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.show();
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
}

void loop() {
  // put your main code here, to run repeatedly:
  EVERY_N_MILLISECONDS(60) {
    showDrips();
  }
}

//function to call the Drip Object's update function,
//moving it and aging it, then actually showing it
void showDrips() {
  fadeToBlackBy(leds, NUM_LEDS, 64);
  for (int i = 0; i < totalDrips; i++) {
    drip[i].updateDrip();
  }
  FastLED.show();
}
