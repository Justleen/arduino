/** 
* reads pH Meter
* updates display
 */
 
#include "header.h"
#include "functions.h"
#include "tasks.h"


Scheduler runner;
//Tasks
Task pH(10000, TASK_FOREVER, &pHCallback, &runner, true);
Task Display(5000, TASK_FOREVER, &displayCallback,  &runner, true);
Task Temp(10000,  TASK_FOREVER, &temperatureCallback,  &runner, true);
Task WiFiConnect(1000, TASK_FOREVER, &WiFiCallback,  &runner, true);
Task NTP(10000,  TASK_FOREVER, &NTPCallback);




void setup () 
{
  Serial.begin(115200);
  // onewire
  sensors.begin();
  //oled
  u8g2.begin();
  //taskScheduler
  runner.startNow();
  // runner.init();
  
  

  
  int addr = 0;
  float pHValueEEPROM = readEEPROM(addr);
  if ( pHValueEEPROM != 0 )
  {
    writeEEPROM(7.0, addr);
  }
}

void loop ()
{
  runner.execute();
}
