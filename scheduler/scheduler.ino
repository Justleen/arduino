/** 
* reads pH Meter
* updates display
 */
 
#include "header.h"
#include "functions.h"
#include "tasks.h"


//Tasks
Task pH(10000, TASK_FOREVER, &pHCallback);
Task Display(5000, TASK_FOREVER, &displayCallback);
Task Temp(10000,  TASK_FOREVER, &temperatureCallback);

Scheduler runner;


void setup () 
{
  Serial.begin(115200);
  // create flash
  // onewire
  sensors.begin();
  //oled
  u8g2.begin();
  //taskScheduler
  runner.init();
  
  
  runner.addTask(pH);
  runner.addTask(Display);
  runner.addTask(Temp);
  Serial.println("added Tasks");
  
  delay(2000);

  pH.enable();
  Display.enable();
  Temp.enable();
  Serial.println("Enabled Tasks");
  writeEEPROM(pHValue, 0);
}

void loop ()
{
  runner.execute();
  readEEPROM(0);
}
