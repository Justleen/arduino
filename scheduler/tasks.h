

// contstruct callbacks
void WiFiCallback();
void NTPCallback();
void temperatureCallback();
void pHCallback();
void displayCallback();

Scheduler runner;
//Tasks
Task pH(10000, TASK_FOREVER, &pHCallback, &runner, true);
Task Display(5000, TASK_FOREVER, &displayCallback,  &runner, true);
Task Temp(10000,  TASK_FOREVER, &temperatureCallback,  &runner, true);
Task WiFiConnect(1000, TASK_FOREVER, &WiFiCallback,  &runner, true);
Task NTP(NTPDELAY,  TASK_FOREVER, &NTPCallback);


void pHCallback()
{
  getpH();
  Serial.print("pH is: ");
  Serial.println(pHValue);
}

void temperatureCallback()
{
  // Temp
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.print("temperature is: ");
  Serial.println(temperature);
}

void displayCallback()
{
  drawDisplay( temperature,  pHValue,  voltage,  WiFiStatus);
  Serial.println("Updating Display");
}

void NTPCallback()
{
  if (WiFi.status() == WL_CONNECTED) {
    //get a random server from the pool
    WiFi.hostByName(ntpServerName, timeServerIP);

    sendNTPpacket(timeServerIP); // send an NTP packet to a time server
    // wait to see if a reply is available
    setCallback(&displayCallback);
    delay(2000);
   
    ntpDate();
  } else {
    runner.addTask(WiFiConnect);
    WiFiConnect.enable();
    runner.deleteTask(NTP);
  }
}

void WiFiCallback()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("no wifi, connecting");
    connectWiFi();
  } else {
    Serial.println(WiFi.localIP());
    runner.addTask(NTP);
    NTP.enable();
    runner.deleteTask(WiFiConnect);
    }
}

void readEEPROMCallback()
{
  readEEPROM(0);
}
