/* Fido.ino
Christopher Datsikas
Assignment 8 DPI - Nov 29 2017
 
 Connect one end of FSR to 5V, the other end to Analog 0.
 Then connect one end of a 10K resistor from Analog 0 to ground
 
 For more information see www.ladyada.net/learn/sensors/fsr.html */
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

//  Variable Definitions
#define NUMPIXELS      1   
#define PIN            6

#define FORCE_5V_PIN   4
#define SWITCH_PIN     9
#define SOLENOID_PIN   10
#define VIBE_PIN       13

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); 

int fsrAnalogPin = 0; // FSR is connected to analog 0
int fsrReading;      // the analog reading from the FSR resistor divider
int buttonState;

void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  
  // Active internal pull-up for switch pin
  pinMode(SWITCH_PIN, OUTPUT);
  digitalWrite(SWITCH_PIN,HIGH);
  
  // Activate internal pull-up resistor to create 5V for the force sensor
  pinMode(FORCE_5V_PIN, OUTPUT);
  digitalWrite(FORCE_5V_PIN,HIGH);

  pixels.begin(); // This initializes the NeoPixel library.'
  // initialize all the neopixels as on, red
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(25,25,25)); // Moderately bright red color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(1000);
  // initialize all the neopixels as on, red
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright red color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  Serial.println("Setup Complete");
}

void loop(void) {
  
  // read the state of the force sensor
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  
  // read the state of the switch
  buttonState = digitalRead(SWITCH_PIN);
  Serial.print("Digital reading = ");
  Serial.println(buttonState);

  //Enter correct state
  // if user is present and not procrastinating (based on switch)
  if (fsrReading > 400 & buttonState == 1) {
    Serial.println("In Condition ONE - ON in standby state");
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(25,25,25)); // White light
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
  // if user is present and procrastinating (based on switch)
  else if (fsrReading > 400 & buttonState == 0) {
    Serial.println("In Condition TWO - Procrastiation State");
    fade(10);
  }
  // user is not present
  else if (fsrReading < 400) {
        Serial.println("In Condition THREE - OFF");
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Off
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }

  delay(10);
}


void fade(uint8_t wait) {
  for (int i = 0; i < 100; i++){
    for (int thisPin = 0; thisPin < NUMPIXELS; thisPin++) {   // iterate through all the levers                               // if lever is pulled down, turn green
      pixels.setPixelColor(thisPin, pixels.Color(i,0,0));   // Moderately bright green color.
      pixels.show();                                         // Update pixel color.
    }
    delay(wait);
  }
  for (int i = 100; i > 0; i--){
    for (int thisPin = 0; thisPin < NUMPIXELS; thisPin++) {   // iterate through all the levers                               // if lever is pulled down, turn green
      pixels.setPixelColor(thisPin, pixels.Color(i,0,0));   // Moderately bright green color.
      pixels.show();                                         // Update pixel color.
    }
    delay(wait);
  }
}

