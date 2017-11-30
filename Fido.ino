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
#define VIBE_PIN       12

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); 

int fsrAnalogPin = 0; // FSR is connected to analog 0
int fsrReading;      // the analog reading from the FSR resistor divider
int buttonState;    // 0 - Button is in the ON position - user is procrastinating
// 1 - Button is in the OFF position - user is NOT procrastinating

void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor

  // Active internal pull-up for switch pin
  pinMode(SWITCH_PIN, OUTPUT);
  digitalWrite(SWITCH_PIN,HIGH);

  // Activate internal pull-up resistor to create 5V for the force sensor
  pinMode(FORCE_5V_PIN, OUTPUT);
  digitalWrite(FORCE_5V_PIN,HIGH);
  
  // Initialize and set solenoid to LOW, so Fido is originally upright
  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, HIGH);
  
  // Initialize vibrating motor and set it to off
  pinMode(VIBE_PIN, OUTPUT);
  digitalWrite(VIBE_PIN, LOW);

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
  // if user is not present
  if (fsrReading < 400) {
    Serial.println("In Condition One - OFF");
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Off
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
  // if user is present and not procrastinating (based on switch)  
  else if (fsrReading > 400 & buttonState == 1) {  // On in standby
    Serial.println("In Condition TWO - ON in standby state");
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(25,25,25)); // White light
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
  // if user is present and procrastinating (based on switch)
  else if (fsrReading > 400 & buttonState == 0) {
    Serial.println("In Condition THREE - Procrastiation State");
    
    // 1. Fido slouches
    Serial.println("Part one");
    digitalWrite(SOLENOID_PIN, LOW);
    delay(5000);
    
    // 2. Red light begin to flash
    Serial.println("Part two");
    for (int i = 0; i < 5; i++){     //repeat five times
      fade(15, false);
    }
    
    // 3. Vibrating Motor Buzzes during the red light on period
    Serial.println("Part three");
    fade(15, true);
  }

  delay(10);
}


void fade(uint8_t wait, boolean buzz) {
  for (int i = 0; i < 100; i++){
    for (int thisPin = 0; thisPin < NUMPIXELS; thisPin++) {   // iterate through all the levers                               // if lever is pulled down, turn green
      pixels.setPixelColor(thisPin, pixels.Color(i,0,0));   // Moderately red color.
      pixels.show();                                         // Update pixel color.
    }
    delay(wait);
  }
  if (buzz == true) {
    digitalWrite(VIBE_PIN, HIGH);
    delay(1000);
    digitalWrite(VIBE_PIN, LOW);
  } 
  for (int i = 100; i > 0; i--){
    for (int thisPin = 0; thisPin < NUMPIXELS; thisPin++) {   // iterate through all the levers                               // if lever is pulled down, turn green
      pixels.setPixelColor(thisPin, pixels.Color(i,0,0));   // Moderately  color.
      pixels.show();                                         // Update pixel color.
    }
    delay(wait);
  }
}


