
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

// void WEBServerCallback();
// Task WEBServer(1000,  1, &WEBServerCallback);

// webserver
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);    // create a websocket server on port 81

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
void handleFileUpload();                // upload a new file to the SPIFFS
bool rainbow = false;             // The rainbow effect is turned off on startup
#define LED_RED     15            // specify the pins with an RGB LED connected
#define LED_GREEN   12
#define LED_BLUE    13

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}


// void WEBServerCallback()
// {

// }

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        rainbow = false;                  // Turn rainbow off when a new connection is established
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] get Text: %s\n", num, payload);
      if (payload[0] == '#') {            // we get RGB data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);   // decode rgb data
        int r = ((rgb >> 20) & 0x3FF);                     // 10 bits per color, so R: bits 20-29
        int g = ((rgb >> 10) & 0x3FF);                     // G: bits 10-19
        int b =          rgb & 0x3FF;                      // B: bits  0-9

        analogWrite(LED_RED,   r);                         // write it to the LED output pins
        analogWrite(LED_GREEN, g);
        analogWrite(LED_BLUE,  b);
      } else if (payload[0] == 'R') {                      // the browser sends an R when the rainbow effect is enabled
        rainbow = true;
      } else if (payload[0] == 'N') {                      // the browser sends an N when the rainbow effect is disabled
        rainbow = false;
      }
      break;
  }
}

void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
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


void handleLight() { //Handler

String message = "Number of args received:";
message += server.args();            //Get number of parameters
message += "\n";                            //Add a new line

for (int i = 0; i < server.args(); i++) {

message += "Arg Number" + String(i) + " -> ";   //Include the current iteration value
message += server.argName(i) + ": ";     //Get the name of the parameter
message += server.arg(i) + "\n";              //Get the value of the parameter

} 
if ( server.arg("state") == "on") {
	digitalWrite(LED_BUILTIN,LOW); //LED off
}
if ( server.arg("state") == "off") {
	digitalWrite(LED_BUILTIN,HIGH); //LED off
}

server.send(200, "text/plain", message);       //Response to the HTTP request

}