

// contstruct callbacks
void WiFiCallback();
void NTPCallback();
void temperatureCallback();
void pHCallback();
void displayCallback();
void influxCallback();
void OTACallback();

Scheduler runner;
//Tasks
Task pH(10000, TASK_FOREVER, &pHCallback, &runner, true);
Task Display(5000, TASK_FOREVER, &displayCallback,  &runner, true);
Task Temp(10000,  TASK_FOREVER, &temperatureCallback,  &runner, true);
Task WiFiConnect(10000, TASK_FOREVER, &WiFiCallback,  &runner, true);
Task OTA(1000, TASK_FOREVER, &OTACallback);
Task NTP(NTPDELAY,  TASK_FOREVER, &NTPCallback);

void OTACallback()
{
	ArduinoOTA.handle();
}


void pHCallback()
{
	getpH();
	int16_t raw = ads.readADC_SingleEnded(0);
	Serial.print("Raw:");
	Serial.println(raw);
	Serial.print("voltage:");
	Serial.println(voltage, 4);
	Serial.print("pH: ");
	Serial.println(pHValue);
	writeinflux("pH", pHValue);
	writeinflux("pHVoltage", voltage);
}

void temperatureCallback()
{
  // Temp
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.print("temperature is: ");
  Serial.println(temperature);
  writeinflux("value", temperature);

}

void displayCallback()
{
  drawDisplay( temperature,  pHValue,  voltage,  WiFiStatus);
  Serial.println("Updating Display");
}

void NTPCallback()
{
  // if (WiFi.status() == WL_CONNECTED) {
  gettimeofday(&tv, nullptr);
  now = time(nullptr);
  now_ms = millis();
  now_us = micros();

  // localtime / gmtime every second change
  static time_t lastv = 0;
  // if (lastv != tv.tv_sec) {
    lastv = tv.tv_sec;
    Serial.println();
    printTm("localtime", localtime(&now));
    Serial.println();
    printTm("gmtime   ", gmtime(&now));
    Serial.println();
    Serial.println();
  // }

  // time from boot
  Serial.print("clock:");
  Serial.print((uint32_t)tp.tv_sec);
  Serial.print("/");
  Serial.print((uint32_t)tp.tv_nsec);
  Serial.print("ns");

  // time from boot
  Serial.print(" millis:");
  Serial.print(now_ms);
  Serial.print(" micros:");
  Serial.print(now_us);

  // EPOCH+tz+dst
  Serial.print(" gtod:");
  Serial.print((uint32_t)tv.tv_sec);
  Serial.print("/");
  Serial.print((uint32_t)tv.tv_usec);
  Serial.print("us");

  // EPOCH+tz+dst
  Serial.print(" time:");
  Serial.print((uint32_t)now);

  // human readable
  Serial.print(" ctime:(UTC+");
  Serial.print((uint32_t)(TZ * 60 + DST_MN));
  Serial.print("mn)");
  Serial.print(ctime(&now));

  // if no wifi, stop ntping
 // }
 // else {
 // 	runner.addTask(WiFiConnect);
 // 	WiFiConnect.enable();
 // 	runner.deleteTask(NTP);
 // }
}


void WiFiCallback()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("no wifi, connecting");
    connectWiFi();
  } else {
    Serial.println(WiFi.localIP());
    runner.addTask(NTP);
    runner.addTask(OTA);
    
    NTP.enable();
    OTA.enable();
    runner.deleteTask(WiFiConnect);
    }
}

void readEEPROMCallback()
{
  readEEPROM(0);
}
