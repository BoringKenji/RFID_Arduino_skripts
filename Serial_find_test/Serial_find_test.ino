byte message[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
//#define message "Hello World"

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  mySerial.begin(115200);
  
  mySerial.write(message,sizeof(message));
}

void loop()
{
  mySerial.write(message,sizeof(message));

  if (mySerial.available()) {
    int hexIn = mySerial.read();
    Serial.println(hexIn,HEX);
  }
  delay(500);
}
