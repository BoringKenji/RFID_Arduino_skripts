#include <SoftwareSerial.h>
SoftwareSerial portTwo(2, 3);

void setup() {
  Serial.begin(9600);

  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  

  portTwo.begin(9600);
}

void loop() {
  portTwo.println("2");
  Serial.println("2 in hardSerial");
  delay(2000);
}
