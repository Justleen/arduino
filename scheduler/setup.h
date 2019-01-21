void setup () 
{
  Serial.begin(115200);
  // create flash
  EEPROM.begin(EEPROMSize);
  // onewire
  sensors.begin();
  //oled
  u8g2.begin();
  //taskScheduler
  runner.init();
  
  
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