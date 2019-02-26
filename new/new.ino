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
	server.begin();                           // Actually start the server

	server.on("/upload", HTTP_GET, []() {                 // if the client requests the upload page
    if (!handleFileRead("/upload.html"))                // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.on("/upload", HTTP_POST,                       // if the client posts to the upload page
    [](){ server.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
    handleFileUpload                                    // Receive and save the file
  );

  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

	server.on("/light", handleLight); //Associate the handler function to the path
	Serial.println("HTTP server started");

}

void loop()
{
	runner.execute();
	server.handleClient();
}