#include "header.h"


int digits=1;


void setup(void) {
  Serial.begin(115200);
  
  connectWiFi();
  udp.begin(localPort);
  u8g2.begin();
  sensors.begin();
  EEPROM.begin(1);
}

void loop(void) {
  EEPROM.put(1, 0);

  // getpH();


  u8g2.firstPage();
  do {

    // pH
    u8g2.setFont(u8g2_font_profont17_tf );
    u8g2.setCursor(0, 20);
    u8g2.print(F("pH")); 
    u8g2.setFont(u8g2_font_profont22_tf );
    u8g2.setCursor(64, 20);
    u8g2.print(pHValue, digits);
    //voltage
    u8g2.setFont(u8g2_font_5x7_mr);
    u8g2.setCursor(0, 30);
    u8g2.print(F("Voltage"));
    u8g2.setCursor(64, 30);
    u8g2.print(voltage, digits);
    
    // Temp
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);

    u8g2.setFont(u8g2_font_profont17_tf );
    u8g2.setCursor(0, 50);
    u8g2.print(F("Temp.")); 
    u8g2.setFont(u8g2_font_profont22_tf );
    u8g2.setCursor(64, 50);
    u8g2.print(temperature, digits); 

    // show IP
    u8g2.setFont(u8g2_font_5x7_mr);
    u8g2.setCursor(0, 64);
    if (WiFi.status() != WL_CONNECTED) {
      u8g2.print("no wifi");
    } else {
      u8g2.print(WiFi.localIP());
    }

  } while ( u8g2.nextPage() );
  // ntpDate();

  delay(1500);

}
