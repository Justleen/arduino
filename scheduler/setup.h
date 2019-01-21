void setup () 
{
  Serial.begin(115200);
  EEPROM.begin(EEPROMSize);
  sensors.begin();
  Serial.println("Scheduler TEST");
  
  u8g2.begin();
  Serial.println("starting Display");
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(pH);
  Serial.println("added pH");
  
  runner.addTask(Display);
  Serial.println("added Display");

  runner.addTask(Temp);
  Serial.println("added Temp");
  


  delay(2000);
  

  pH.enable();
  Serial.println("Enabled pH");

  Display.enable();
  Serial.println("Enabled Display");

  Temp.enable();
  Serial.println("Enabled Temp");
}