// Callback methods prototypes
// void pHCallback();
// void displayCallback();
// void temperatureCallback();


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

void WiFiCallback()
{
	if (WiFi.status() != WL_CONNECTED) {
      	Serial.println("no wifi, connecting");
		connectWiFi();
    } else {
      Serial.println(WiFi.localIP());
    }

}
void NTPCallback()
{
	ntpDate();
}

void readEEPROMCallback()
{
	readEEPROM(0);
}