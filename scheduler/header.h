#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <TaskScheduler.h>

//WiFi
#ifndef STASSID
#define STASSID "koepelnet"
#define STAPSK  "fluffy22"
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;


//NTP
unsigned int localPort = 2390;      // local port to listen for UDP packets
/* Don't hardwire the IP address or we won't get the benefits of the ntp pool.
    Lookup the IP address for the host name instead */
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "0.pool.ntp.org";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
int NTPDELAY = 1000 ;

 
// temperatue DS18
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


//  analog Ph meter
#define samplingInterval 20
#define ArrayLenth  40    //times of collection
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;   

// oled
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int digits = 1; // number of decimals

//globals ( OO would be nice... )
float temperature;
float pHValue;
float voltage;
int WiFiStatus = 0;


// EEPROM
int EEPROMSize = 512;

struct EEPROMObject {
  float pH;
  float voltage;
  char name[10];
};