/** 
* reads pH Meter
* updates display
 */
 
#include "header.h"
#include "functions.h"
#include "tasks.h"


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
