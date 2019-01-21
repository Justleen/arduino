/** 
* reads pH Meter
 */
 
 
#include <TaskScheduler.h>
#include "header.h"
#include "functions.h"

// Callback methods prototypes
void pHCallback();

void displayCallback();
//Tasks


Task pH(10000, TASK_FOREVER, &pHCallback);
Task Display(5000, TASK_FOREVER, &displayCallback);

Scheduler runner;


void pHCallback() {
    float pHValue = getpH();
    Serial.print("pH is: ");
    Serial.println(pHValue);
}

void displayCallback(){
  drawDisplay(33.0, 7.1, 3.3, 0);
  Serial.println("Updating Display");
}

void setup () {
  Serial.begin(115200);
  Serial.println("Scheduler TEST");
  
  u8g2.begin();
  Serial.println("starting Display");
  runner.init();
  Serial.println("Initialized scheduler");
  
 
  runner.addTask(pH);
  Serial.println("added pH");
  
  runner.addTask(Display);
  Serial.println("added Display");
  

  delay(5000);
  
  pH.enable();
  Serial.println("Enabled pH");

  Display.enable();
  Serial.println("Enabled Display");

}


void loop () {
  runner.execute();
}