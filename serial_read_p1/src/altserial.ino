#include <AltSoftSerial.h>

 
AltSoftSerial altSerial;
char c;
 
void setup() {
    Serial.begin(115200);
    altSerial.begin(115200);
}
 
void loop() {
    if (altSerial.available()) {
        c = Serial.read();
        // --- 7 bits instelling ---
        c &= ~(1 << 7);
        char inChar = (char)c;
    Serial.print(c);
    }
}


