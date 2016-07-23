/* Arduino 'slimme meter' P1-port reader.
 
 This sketch reads data from a Dutch smart meter that is equipped with a P1-port.
 Connect 'RTS' from meter to Arduino pin 5
 Connect 'GND' from meter to Arduino GND
 Connect 'RxD' from meter to Arduino pin 0 (RX)
 
 Baudrate 115200, 8N1.
 BS170 transistor & 10k resistor is needed to make data readable if meter spits out inverted data
 
 A .php file is requested (with consumption numbers in the GET request) every minute (interval set at line #52)
 created by 'ThinkPad' @ Tweakers.net, september 2014
 
 http://gathering.tweakers.net/forum/list_messages/1601301
 */

#include <AltSoftSerial.h>


// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Teensy 2.0         9        10       (none)
// Teensy++ 2.0      25         4       26, 27
// Arduino Uno        9         8         10
// Arduino Mega      46        48       44, 45
// Wiring-S           5         6          4
// Sanguino          13        14         12


const int requestPin =  5;         
char input; // incoming serial data (byte)
bool readnextLine = false;
#define BUFSIZE 75
char buffer[BUFSIZE]; //Buffer for serial data to find \n .
int bufpos = 0;
long mEVLT = 0; //Meter reading Electrics - consumption low tariff
long mEVHT = 0; //Meter reading Electrics - consumption high tariff
long mEAV = 0;  //Meter reading Electrics - Actual consumption
long mG = 0;   //Meter reading Gas

long lastTime = 0;        // will store last time 
long interval = 60000;           // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(115200);
  delay(1000);

  //Set RTS pin high, so smart meter will start sending telegrams
  pinMode(requestPin, OUTPUT);
  digitalWrite(requestPin, HIGH);
}

void loop() {

  decodeTelegram();
  Serialrequest();
  if(millis() - lastTime > interval) {
    lastTime = millis();   

  }
} //Einde loop

void decodeTelegram() {
  long tl = 0;
  long tld =0;

  if (Serial.available()) {
    input = Serial.read();
    char inChar = (char)input;
    // Fill buffer up to and including a new line (\n)
    buffer[bufpos] = input&127;
    bufpos++;

    if (input == '\n') { // We received a new line (data up to \n)
      if (sscanf(buffer,"1-0:1.8.1(%ld.%ld" ,&tl, &tld)==2){
        tl *= 1000;
        tl += tld;
        mEVLT = tl;
      }

      // 1-0:1.8.2 = Elektra verbruik hoog tarief (DSMR v4.0)
      if (sscanf(buffer,"1-0:1.8.2(%ld.%ld" ,&tl, &tld)==2){
        tl *= 1000;
        tl += tld;
        mEVHT = tl;
      }

      // 1-0:1.7.0 = Electricity consumption actual usage (DSMR v4.0)
      if (sscanf(buffer,"1-0:1.7.0(%ld.%ld" ,&tl , &tld) == 2)
      { 
        mEAV = (tl*1000)+tld;
      }

      // 0-1:24.2.1 = Gas (DSMR v4.0) on Kaifa MA105 meter
      if (strncmp(buffer, "0-1:24.2.1", strlen("0-1:24.2.1")) == 0) {
        if (sscanf(strrchr(buffer, '(') + 1, "%d.%d", &tl, &tld) == 2) {
          mG = (tl*1000)+tld; 
        }
      }

      // Empty buffer again (whole array)
      for (int i=0; i<75; i++)
      { 
        buffer[i] = 0;
      }
      bufpos = 0;
    }
  } //Einde 'if AltSerial.available'
} //Einde 'decodeTelegram()' functie


void Serialrequest() {
  // if there's a successful connection:
    Serial.print(mEVLT);
    Serial.print(mEVHT);
    Serial.print(mEAV);
    Serial.print(mG);
    Serial.println();
}
