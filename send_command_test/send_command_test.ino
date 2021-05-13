#define led 13
int val = 0;
byte data[5];  //For holding the ID we receive

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Start Serial communication");
  
  mySerial.begin(115200);
  //mySerial.println("Hello, world?");

  byte message[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
  mySerial.write(message, sizeof(message));

}

void loop() {
  if(Serial.available())
  {
    char letter = Serial.read();
    if(letter == '1'){
      digitalWrite(led,HIGH);
      byte message[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
      mySerial.write(message, sizeof(message));
    }
    if(letter == '2'){
      digitalWrite(led,HIGH);
      Serial.println("BB 00 22 00 00 22 7E ");
      mySerial.println("BB 00 22 00 00 22 7E ");
    }
    if(letter == '3'){
      digitalWrite(led,HIGH);
      //Serial.println("BB 00 27 00 03 22 FF FF 4A 7E");
      mySerial.println("BB 00 27 00 03 22 FF FF 4A 7E");
    }
    else if(letter == '0'){
      digitalWrite(led,LOW); 
      //Serial.println("LED is OFF");
      mySerial.println("LED is OFF");
    }
  }
  
  val = mySerial.read();
   while (val != 0xBB)
   {  //On Successful read, first byte will always be 0xFF
      val = mySerial.read();
      delay(1000);
   }
   
   mySerial.read();              // reserved
   mySerial.read();              // length
   mySerial.read();              // command (indicates tag data)
   data[0] = mySerial.read();    // we read data 1
   data[1] = mySerial.read();    // we read data 2
   data[2] = mySerial.read();    // we read data 2

   Serial.println(data[0]);
}
