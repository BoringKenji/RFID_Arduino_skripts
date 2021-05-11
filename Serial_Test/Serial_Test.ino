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
  
  mySerial.begin(9600);
  mySerial.println("Hello, world?");

}

void loop() {
  if(Serial.available())
  {
    char letter = Serial.read();
    if(letter == '1'){
      digitalWrite(led,HIGH);
      Serial.println("LED is ON");
      mySerial.println("LED is ON");
    }
    else if(letter == '0'){
      digitalWrite(led,LOW); 
      Serial.println("LED is OFF");
      mySerial.println("LED is OFF");
    }
  }
  
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}
