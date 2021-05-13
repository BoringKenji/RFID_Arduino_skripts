#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX


//Commands
byte readMulti[] = {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0xFF, 0xFF, 0x4A, 0x7E};
byte readSingle[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
byte stopRead[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};


void printWelcomeMessage(void){
  Serial.println("Choose from the Menue and enter the number:");
  Serial.println("1. Single Read");
  Serial.println("2. Multi Read (buggy)");
  Serial.println("3. Stop Read (not wroking)");
  Serial.println("5. Print Menue");
  Serial.println();
}


void setup() {
  Serial.begin(9600);
  Serial.println("Serial started");
  
  mySerial.begin(115200);

  
  printWelcomeMessage();
}


  
void loop() {
  //read commands from Serial Monitor
  if(Serial.available())
  {
    char letter = Serial.read();
    if(letter == '1'){
      mySerial.write(readSingle, sizeof(readSingle));
    }
    if(letter == '2'){
      mySerial.write(readMulti, sizeof(readMulti));
    }
    if(letter == '3'){
      mySerial.write(stopRead, sizeof(stopRead));
    }
    if(letter == '4'){
      mySerial.write(readSingle, sizeof(readSingle));
    }
    else if(letter == '5'){
      printWelcomeMessage();
    }
  }

  //print RFID reply 
  while (mySerial.available()) {
    int hexIn = mySerial.read();
    Serial.print(hexIn,HEX);
    if(hexIn == 0x7E)
      Serial.println();   
  }
}



  
