#include <TaskScheduler.h>
#include <FS.h>
File fsUploadFile;              // a File object to temporarily store the received file
Scheduler runner;

#include "webserver.h"
#include "wifi.h"


void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN,HIGH);
	Serial.begin(115200);
	SPIFFS.begin();
	runner.startNow();
}

void loop()
{
	runner.execute();
	server.handleClient();
}