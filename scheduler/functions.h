
void writeEEPROM(float input, int addr)
{
  EEPROM.begin(EEPROMSize);
  EEPROM.put(addr, input);
  EEPROM.end();
}

float readEEPROM(int addr)
{
  float value;
  EEPROM.begin(EEPROMSize);
  EEPROM.get(addr, value );
  EEPROM.end();
  Serial.println(value);
}

double avergearray(int* arr, int number)
{
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

float getpH()
{
  for (int i=0; i <= int(ArrayLenth); i++)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*3.2/1023;
      // pHx = pH1 + (Ex – E1)*(pH2 – pH1)/(E2-E1)
      // pHValue = sampleOne + (y - voltsampleOne ) * (sampleTwo - sampleOne) / ( voltsampleTwo - voltpHSeven)
      pHValue =  22.06  - 6  * voltage;
      delay(int(samplingInterval));
  }
  // return pHValue;
}

void drawDisplay(float temperature, float pHValue, float voltage, int WiFiStatus) 
{
  u8g2.firstPage();
  do {

    // pH
    u8g2.setFont(u8g2_font_profont17_tf );
    u8g2.setCursor(0, 20);
    u8g2.print(F("pH")); 
    u8g2.setFont(u8g2_font_profont22_tf );
    u8g2.setCursor(64, 20);
    u8g2.print(pHValue, digits);
    //voltage
    u8g2.setFont(u8g2_font_5x7_mr);
    u8g2.setCursor(0, 30);
    u8g2.print(F("Voltage"));
    u8g2.setCursor(64, 30);
    u8g2.print(voltage, digits);
    
    // Temp

    u8g2.setFont(u8g2_font_profont17_tf );
    u8g2.setCursor(0, 50);
    u8g2.print(F("Temp.")); 
    u8g2.setFont(u8g2_font_profont22_tf );
    u8g2.setCursor(64, 50);
    u8g2.print(temperature, digits); 

    // show IP
    u8g2.setFont(u8g2_font_5x7_mr);
    u8g2.setCursor(0, 64);
    if (WiFiStatus != 0) {
      u8g2.print("no wifi");
    } else {
      u8g2.print(millis());
    }

  } while ( u8g2.nextPage() );
}