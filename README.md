# arduino IDE, ESP8266 Board

Wemos D1 mini "aquaphonics" pH meter, temperature, Wifi, NTP, OLED

Based on Task Scheduler

// ph meter
analog meter, https://www.dfrobot.com/wiki/index.php/PH_meter(SKU:_SEN0161)


// thermometer
Generic Aliexpress waterproof DS18d20, OneWire

// Wemos
https://wiki.wemos.cc/products:d1:d1_mini

//OLED
cheapest 1.34" , again, on Alie, i2C, sdd1106


// maths stuffs, for science

1. Choose two buffers, let say pH1 = 7.00 and pH2 = 4.00 and measure the corresponding E1 and E2 (in mV).
2. For a sample with pHx, you measure Ex.
3. Calculate pHx = pH1 + (Ex – E1)(pH2 – pH1)/(E2-E1)
Important notes:
- all buffers and samples must be measured at the same temperature
- use a stabilisation time of about 1 minute for the electrode response
- the instrument must have a very high impedance (500 Gohm, not used in common voltmeters; use a pH/mV-meter in "mV" mode to test the equation)
- use a 3rd known buffer as sample (e.g., pH= 9.00 or 10.00) to verify if the sensor response is linear).
- at pH=7.00 , the common measured E is not zero; in the given equation, (Ex – E1) is the correction for this asymmetry
- in the given equation, (pH2 – pH1)/(E2-E1) is the electrode slope.
