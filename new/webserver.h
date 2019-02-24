
#include <ESP8266WebServer.h>

void WEBServerCallback();
Task WEBServer(1000,  1, &WEBServerCallback);

// webserver
ESP8266WebServer server(80);


String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
void handleFileUpload();                // upload a new file to the SPIFFS


String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
	if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
	  path += ".gz";                                         // Use the compressed verion
	File file = SPIFFS.open(path, "r");                    // Open the file
	size_t sent = server.streamFile(file, contentType);    // Send it to the client
	file.close();                                          // Close the file again
	Serial.println(String("\tSent file: ") + path);
	return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleFileUpload(){ // upload a new file to the SPIFFS

  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
	String filename = upload.filename;
	if(!filename.startsWith("/")) filename = "/"+filename;
	Serial.print("handleFileUpload Name: "); Serial.println(filename);
	fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
	filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
	if(fsUploadFile)
	  fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
	if(fsUploadFile) {                                    // If the file was successfully created
	  fsUploadFile.close();                               // Close the file again
	  Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
	  server.sendHeader("Location","/success.html");      // Redirect the client to the success page
	  server.send(303);
	} else {
	  server.send(500, "text/plain", "500: couldn't create file");
	}
  }
}

void handleLightOn()
{
	Serial.println("LED on page");
	server.send(200, "text/html", "LED is ON"); //Send ADC value only to client ajax request
	digitalWrite(LED_BUILTIN,LOW); //LED is connected in reverse
}

void handleLightOff()
{
	Serial.println("LED off page");
	digitalWrite(LED_BUILTIN,HIGH); //LED off
	server.send(200, "text/html", "LED is OFF"); //Send ADC value only to client ajax request
}

void WEBServerCallback()
{
  server.on("/upload", HTTP_GET, []() {                 // if the client requests the upload page
	if (!handleFileRead("/upload.html"))                // send it if it exists
	  server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.on("/upload", HTTP_POST,                       // if the client posts to the upload page
	[](){ server.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
	handleFileUpload                                    // Receive and save the file
  );

	server.on("/on", handleLightOn);
	server.on("/off", handleLightOff);

	server.onNotFound([]() {                              // If the client requests any URI
	if (!handleFileRead(server.uri()))                  // send it if it exists
		server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
	});
	server.begin();
	Serial.println("HTTP server started");

}