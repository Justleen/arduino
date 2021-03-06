/** 
* reads pH Meter
* updates display
 */

#include "credentials.h"
#include "header.h"
#include "functions.h"
#include "webhandlers.h"
#include "tasks.h"


#define LED 2

void setup () 
{
  influx.setDb(INFLUXDB_DATABASE);

  
  Serial.begin(115200);
  //NTP
  settimeofday_cb(time_is_set);
  configTime(TZ_SEC, DST_SEC, "pool.ntp.org");
  //ADS115
  ads.begin();
  ads.setGain(GAIN_ONE);       
  // onewire
  sensors.begin();
  // oled
  u8g2.begin();
  //taskScheduler
  runner.startNow();
  // runner.init();
  
  // lights
  pinMode(LED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(whiteLED, OUTPUT);
  analogWrite(whiteLED, 255);


  // OTA over the air updates
  ArduinoOTA.onStart([]() {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH) 
  {
    type = "sketch";
  } else { // U_SPIFFS
    type = "filesystem";
  }

  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() 
  {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) 
  {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
   
  ArduinoOTA.begin();
  SPIFFS.begin();

  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);

}

void loop ()
{
  runner.execute();
  server.handleClient();
  MDNS.update();

}
