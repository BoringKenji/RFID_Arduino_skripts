#include <SoftwareSerial.h>
SoftwareSerial portOne(11, 12);

void setup() {
  Serial.begin(9600);

  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  

  portOne.begin(9600);
}

void loop() {
  portOne.println("1");
  Serial.println("1 in hardSerial");
  delay(1000);
}
