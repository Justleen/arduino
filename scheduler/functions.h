
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
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*3.2/1023;
      // pHx = pH1 + (Ex – E1)*(pH2 – pH1)/(E2-E1)
      // pHValue = sampleOne + (y - voltsampleOne ) * (sampleTwo - sampleOne) / ( voltsampleTwo - voltpHSeven)
      pHValue =  22.06  - 6  * voltage;
      delay(int(samplingInterval));
  }
  // return pHValue;
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
    u8g2.print(voltage, digits);
    
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
  //   if (WiFiStatus != 0) {
  //     u8g2.print("no wifi");
  //   } else {
  //     u8g2.print(millis());
  //   }

  } while ( u8g2.nextPage() );
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void ntpDate() 
{
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  } else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
  // wait ten seconds before asking for the time again
  delay(1000);
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