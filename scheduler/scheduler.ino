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
  //NTP
  settimeofday_cb(time_is_set);
  configTime(TZ_SEC, DST_SEC, "pool.ntp.org");
  // for testing purpose:
  // extern "C" int clock_gettime(clockid_t unused, struct timespec *tp);
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
