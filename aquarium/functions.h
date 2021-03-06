
void writeEEPROM(float input, int addr)
{
  EEPROM.begin(EEPROMSize);
  EEPROM.put(addr, input);
  EEPROM.end();
  Serial.print("wrote to eprom: ");
  Serial.print(input);
  Serial.print(" at address: ");
  Serial.println(addr);
}

void writeinflux(  String device, float value)
{
  if (WiFi.status() == WL_CONNECTED) {
    InfluxData row("aquarium");
    row.addTag("device", device);
    // row.addTag("sensor", "one");
    // row.addTag("mode", "pwm");
    // row.addValue("loopCount", loopCount);
    row.addValue(device, value);

    influx.write(row);
    Serial.print("wrote to influx:");
    Serial.println(device);
  }
}

float readEEPROM(int addr)
{
  float value;
  EEPROM.begin(EEPROMSize);
  EEPROM.get(addr, value );
  EEPROM.end();
  Serial.print("read value: ");
  Serial.print(value);
  Serial.print(" from address: ");
  Serial.println(addr);
}

double avergearray(int* arr, int number)
{
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

float getpH()
{
  for (int i=0; i <= int(ArrayLenth); i++)
  {
      // pHArray[pHArrayIndex++]=analogRead(SensorPin);
      pHArray[pHArrayIndex++]=ads.readADC_SingleEnded(0);


      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      // voltage = avergearray(pHArray, ArrayLenth)*3.2/1023;
      raw = avergearray(pHArray, ArrayLenth);
      voltage = (raw * gainFactor)/1000;


      // pHx = pH1 + (Ex – E1)*(pH2 – pH1)/(E2-E1)
      // pHOne == shortciruit pH 7
      // phTwo == Calibration liquid pH 4
      // VpHOne == shortciruit voltage 
      // VphTwo == voltage Calibration liquid
      // pHValue = pHOne + (y - VpHOne ) * (phTwo - pHOne) / ( VphTwo - VpHOne)

   
      pHValue = 7 - 5.80159 * (voltage - 2.5179);
      delay(int(samplingInterval));
  }
  // return pHValue;
  // int16_t adc0;

  // adc0 = ads.readADC_SingleEnded(0);
 
  // Serial.print("AIN0: "); Serial.println(adc0);
  // voltage = (adc0 * 0.1875)/1000;
  // pHValue =  22.06  - 6  * voltage;
  // Serial.print("AIN0 V: "); Serial.println(voltage); 
  // Serial.print("AIN0 pH: "); Serial.println(voltage); 
  // Serial.println(" ");
}

void drawDisplay(float temperature, float pHValue, float voltage, int WiFiStatus) 
{
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
    u8g2.print(voltage, 4);
    
    // Temp

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
    u8g2.setCursor(100, 64);
    timeinfo = localtime(&now);
    u8g2.print(timeinfo->tm_hour );
    u8g2.print(":" );
    u8g2.print(timeinfo->tm_min );


  } while ( u8g2.nextPage() );
}

#define PTM(w) \
  Serial.print(":" #w "="); \
  Serial.print(tm->tm_##w);

void printTm(const char* what, const tm* tm) {
  Serial.print(what);
  PTM(isdst); PTM(yday); PTM(wday);
  PTM(year);  PTM(mon);  PTM(mday);
  PTM(hour);  PTM(min);  PTM(sec);
}

void time_is_set(void) {
  gettimeofday(&cbtime, NULL);
  cbtime_set = true;
  Serial.println("------------------ settimeofday() was called ------------------");
}

// connect to wifi network
void connectWiFi() 
{
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
}

void blinkLED()
{
  analogWrite(whiteLED, 50);
  delay(blinkLEDdelay);
  analogWrite(whiteLED, 0);
  delay(blinkLEDdelay);

  analogWrite(whiteLED, 50);
  delay(blinkLEDdelay);
  analogWrite(whiteLED, 0);
  delay(blinkLEDdelay);


}