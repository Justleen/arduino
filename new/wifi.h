#include <ESP8266WiFi.h>
#include "credentials.h"


void WiFiCallback();
Task WiFiConnect(10000, TASK_FOREVER, &WiFiCallback,  &runner, true);

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

void WiFiCallback()
{
  if (WiFi.status() != WL_CONNECTED) {
	Serial.println("no wifi, connecting");
	connectWiFi();
  } else {
	Serial.println(WiFi.localIP());
	// runner.deleteTask(WiFiConnect);

	// runner.addTask(WEBServer);
	// WEBServer.enable();

	}
}
