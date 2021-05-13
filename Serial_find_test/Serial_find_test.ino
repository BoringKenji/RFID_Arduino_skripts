byte message[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
byte data[5];  //For holding the ID we receive
int val = 0;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial started");
  
  mySerial.begin(115200);
  mySerial.write(message,sizeof(message));
  Serial.write(message,sizeof(message));
  
  delay(500);
  while(mySerial.available())
  {   
    Serial.println("somehow never reaches here");
    Serial.println(mySerial.read());
  }
  Serial.println();
  
}

void loop()
{
  while(1){
  Serial.write(message,sizeof(message));
  //Serial.println("has sent the command to RFID");
  delay(500);
  //Serial.println(mySerial.available());
  delay(500);
  }
  
  val = mySerial.read();
  while (val != 0xBB)
  {  //On Successful read, first byte will always be 0xBB
     val = mySerial.read();
     Serial.println((String)"val: " + val);
     delay(1000);
  }

  data[0] =  mySerial.read();    // we read data 1
  Serial.println((String)"data0: " + data[0]);
  data[1] = mySerial.read();    // we read data 2
  data[2] = mySerial.read();    // we read data 3
  data[3] = mySerial.read();    // we read data 4
  data[4] = mySerial.read();    // we read data 5

   
//  while (mySerial.available()) {
//    int hexIn = mySerial.read();
//    Serial.println(hexIn,HEX);
//    delay(0);
//  }
  delay(500);
}
