//  analog Ph meter
#define samplingInterval 20
#define ArrayLenth  40    //times of collection
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;   

// oled
#include <Arduino.h>
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int digits = 1; // number of decimals