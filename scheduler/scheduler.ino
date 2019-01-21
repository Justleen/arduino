/** 
* reads pH Meter
* updates display
 */
 
#include "header.h"
#include "functions.h"

// Callback methods prototypes
void pHCallback();
void displayCallback();
void temperatureCallback();

//Tasks
Task pH(10000, TASK_FOREVER, &pHCallback);
Task Display(5000, TASK_FOREVER, &displayCallback);
Task Temp(10000,  TASK_FOREVER, &temperatureCallback);

Scheduler runner;


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
}


void displayCallback()
{
  drawDisplay( temperature,  pHValue,  voltage,  WiFiStatus);
  Serial.println("Updating Display");
}


#include "setup.h"


void loop ()
{
  runner.execute();
}
