#define led 13

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
  
  if (mySerial.available()) {
//    for (int i = 0; i < 11; i++) { 
//      while (!mySerial.available()); // wait for a character
//
//      int incomingByte = mySerial.read();
//      Serial.print(incomingByte, HEX);
//      Serial.print(' ');
//    }
    
    int hexIn = mySerial.read();
    //Serial.print("I received: ");
    Serial.println(hexIn,HEX);
  }

}
