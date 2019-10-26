#include <FastLED.h>

#define DATA_PIN 2

#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 100
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
#define AVG_AUX_VALS 50
#define MIDDLE NUM_LEDS/2

int brightness = 85;
int aux = A0;
int maxAuxVal = 0;
int avgAuxVal = 0;
int maxAvgAuxVal = 0;
int auxVals[AVG_AUX_VALS] = {0};
int avgIntensity = 0;
int num = 0;
int arraySum = 0;
int m1 = 20;
int m2 = 1;
int valLoc = 0;
int brigntess = 5;
bool rainbowGoing = false;
int rainbowCounter = (NUM_LEDS)-1;
static float hue = 86;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(57600);
	Serial.println("resetting");
  FastLED.addLeds<WS2811, 2>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, 12>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  pinMode(aux, INPUT);
	LEDS.setBrightness(brightness);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() {
  int START_LEDS = 0;
  int END_LEDS = 0;
  hue = hue + 0.1;
  if(hue >= 255){
    hue = 0;
  }
  //brightness = readSignal();
  brightness = 100;
  fromMiddle(brightness, hue, 1000);
  //LEDSout(brightness, hue);
  //delay(1000);
  //LEDSin(brightness);
  //delay(1000);
  //Serial.println("hi");
  /*Serial.print("avg intensity: ");Serial.println(brightness);
  //fromMiddle(brightness, hue);
  //rainbowBOP(brightness, START_LEDS, END_LEDS); // see if rainbow should appear and if it should update it
  /*for(int i = 0; i < START_LEDS; i++){
    leds[i] = CHSV(hue,255,255);
  }
  for(int i = END_LEDS; i < NUM_LEDS; i++){
    leds[i] = CHSV(hue,255,255);
  }*/
  FastLED.show();
  /*
  if(brightness >= 254){
    brightness++;
  } else if (brightness == 1){
    brightness--;
  }
  delay(10);*/
  /*
  LEDS.setBrightness(brightness);
   FastLED.show(); // Show the leds
	for(int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CHSV(255, 255, 255);
    
		
		FastLED.show(); // Show the leds
		fadeall();
		delay(10);
	}

	// Now go in the other direction.  
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		leds[i] = CHSV(hue++, 255, 255);
		
    brightness = readSignal();
    LEDS.setBrightness(brightness);
    Serial.print("avg intensity: ");Serial.println(brightness);
    
		FastLED.show(); // Show the leds
		fadeall();
		delay(10);
	}*/
}

void fromMiddle(int brightness, float hue, float timeOut){
  int outer = brightness/2;
  if(outer <= 0){
    outer = 0;
  }
  int timeGoing = 50;
  int down = 0;
  for(int i = 0; i < outer; i++){ // led expansion
    leds[MIDDLE + i] = CHSV(hue, 255, 255);
    leds[MIDDLE - i] = CHSV(hue, 255, 255);
    FastLED.show();
    delay(timeGoing - (down - 0.01));
    down+=timeGoing/outer;
  }
  delay(timeOut);
  for(int i = outer; i > 0; i--){ // led contraction
    leds[MIDDLE + i] = CHSV(hue, 255, 0);
    leds[MIDDLE - i] = CHSV(hue, 255, 0);
    FastLED.show();
    delay(0.5);
  }
  
}

// funcs for pulsing strip out and in
void LEDSout(int brightness, float hue){
  int upper = MIDDLE + brightness/2;
  int lower = MIDDLE - brightness/2;
  if(lower < 0){
    lower = 0;
  }
  if(upper > 199){
    upper = 199;
  }
  
  for(int i = MIDDLE; i < upper; i++){ // starts middle and goes left to 199
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
  }

  for(int i = MIDDLE; i > lower; i--){ // starts middle and goes right to 0
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
  }
}

void LEDSin(int brightness){
  int upper = MIDDLE + brightness/2;
  int lower = MIDDLE - brightness/2;
  if(lower < 0){
    lower = 0;
  }
  if(upper > 199){
    upper = 199;
  }
  
  for(int i = upper; i > MIDDLE; i++){ // starts left and goes to middle
    leds[i] = CHSV(hue, 255, 0);
    FastLED.show();    
  }

  for(int i = lower; i < MIDDLE; i--){ // starts right and goes to middle
    leds[i] = CHSV(hue, 255, 0);    
    FastLED.show();
  }
}

void setBrightnessMain(int brightness){ // sets brightness for main leds based on readsignal
  if(brightness < 10){
    brightness = 0;
  }
  LEDS.setBrightness(brightness);
}

void rainbowBOP(int intensity, int &START_LEDS, int &END_LEDS){ // makes rainbows appear above a certain threshold
  // add timer and each counter is an array then do a for loop through arrays
  if(intensity > 170 && rainbowGoing == false){ // only one rainbow at a time rn
    rainbowGoing = true; // set variables for this rainbow
  }

  if(rainbowGoing == true){
    rainbowCounter--;
    START_LEDS = rainbowCounter;
    END_LEDS = rainbowCounter - 30;
    leds[rainbowCounter] = CHSV(hue++, 255, 255);
    
    LEDS.setBrightness(255);
    
    fadeall();
    delay(1);
    Serial.print("rainbowCounter");Serial.println(rainbowCounter);
    if(rainbowCounter == 0){
      rainbowGoing = false;
      rainbowCounter = (NUM_LEDS)-1;
    }
  }
}

int readSignal() {
  int auxValue = analogRead(aux);
  if(auxValue >= maxAuxVal){ // find max aux val
    maxAuxVal = auxValue;
  }
  if(avgAuxVal >= maxAvgAuxVal){
    maxAvgAuxVal = avgAuxVal; // find max avg aux val - use to map to max brightness (needs to change over time)
  }
  
  if(num < AVG_AUX_VALS){ // to fill up array
    auxVals[num] = auxValue;
    num++;
  }
  
  // to replace values after array has filled up
  auxVals[valLoc] = auxValue;
  
  arraySum = 0;
  for(int i = 0; i < AVG_AUX_VALS; i++){
    arraySum+=auxVals[i];
  }

  avgAuxVal = arraySum/sizeof(auxVals);
  
  /*
  Serial.print("auxValue: ");Serial.print(auxValue);
  Serial.print("\tmaxAuxValue: ");Serial.print(maxAuxVal);
  Serial.print("\taverageAuxValue: ");Serial.println(avgAuxVal);
  */
  
  if(valLoc <= AVG_AUX_VALS){
    valLoc++;
  } else {
    valLoc = 0;
  }
  avgIntensity = map(avgAuxVal, 0, 20, 0, 255); // change 30 to maxAvgAuxVal so max intensity changes with song
  return avgIntensity;
}
