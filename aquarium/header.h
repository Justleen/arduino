#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <TaskScheduler.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include <InfluxDb.h>

#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>


#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
File fsUploadFile;              // a File object to temporarily store the received file
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
void handleFileUpload();                // upload a new file to the SPIFFS

//ADS1115
Adafruit_ADS1115 ads;
float gainFactor = 0.125;

// lights
#define whiteLED D6
#define blinkLEDdelay  500
 
//WiFi
#ifndef STASSID
#define STASSID "koepelnet"
#define STAPSK  "fluffy22"
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;
int WiFiStatus = 0;


//NTP
#define TZ              0       // (utc+) TZ in hours
#define DST_MN          60      // use 60mn for summer time in some countries
#define TZ_MN           ((TZ)*60)
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)
timeval cbtime;			// time set in callback
bool cbtime_set = false;
timeval tv;
timespec tp;
time_t now;
uint32_t now_ms, now_us;
int NTPDELAY= 60000;
struct tm * timeinfo;
 
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
//globals ( OO would be nice... )
float temperature;
float pHValue;
float voltage;
int raw;


// oled
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int digits = 1; // number of decimals

//Influx
// #define INFLUXDB_HOST "monitoring.aardehuis.nl"
#define INFLUXDB_HOST "192.168.178.169"
#define INFLUXDB_PORT "8086"
#define INFLUXDB_DATABASE "test"
#define INFLUXDB_USER  "leen"
#define INFLUXDB_PASS  "ppQNwwOXN005kwgO94NC"
Influxdb influx(INFLUXDB_HOST); // port defaults to 8086
// influx.setDbAuth('aqaurium', INFLUXDB_USER, INFLUXDB_PASS);

 //if used with authentication
// #include "credentials.h"

// EEPROM
int EEPROMSize = 512;

struct EEPROMObject {
  float pH;
  float voltage;
  char name[10];
};


