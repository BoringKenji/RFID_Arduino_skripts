#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX


void setup() {
  Serial.begin(9600);
  Serial.println("Serial started");
  
  mySerial.begin(115200);

}

void loop() {
  while (mySerial.available()) {
    int hexIn = mySerial.read();
    Serial.print(hexIn,HEX);
    if(hexIn == 0x7E)
      Serial.println();
    
  }
}
