

// color swirl! connect an RGB LED to the PWM pins as indicated
// in the #defines
// public domain, enjoy!
 
// #define REDPIN D5
// #define GREENPIN D6
#define BLUEPIN D6
 
#define FADESPEED 50     // make this higher to slow down
 
void setup() {
  // pinMode(REDPIN, OUTPUT);
  // pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}
 
 
void loop() {

  int r, g, b;
  for (b = 255; b > 0; b--) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  for (b = 0; b < 256; b++) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
 
  // // fade from blue to violet
  // for (r = 0; r < 256; r++) { 
  //   analogWrite(REDPIN, r);
  //   delay(FADESPEED);
  // } 
  // fade from violet to red
  // // fade from red to yellow
  // for (g = 0; g < 256; g++) { 
  //   analogWrite(GREENPIN, g);
  //   delay(FADESPEED);
  // } 
  // // fade from yellow to green
  // for (r = 255; r > 0; r--) { 
  //   analogWrite(REDPIN, r);
  //   delay(FADESPEED);
  // } 
  // fade from green to teal
  // // fade from teal to blue
  // for (g = 255; g > 0; g--) { 
  //   analogWrite(GREENPIN, g);
  //   delay(FADESPEED);
  // } 
  analogWrite(BLUEPIN, 255);
  delay(60000);
  analogWrite(BLUEPIN, 255);
}