#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX

String getRFIDData(void);

//Commands
byte readSingle[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
byte readMulti[] = {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0x00, 0x0A, 0x56, 0x7E};
byte stopRead[] = {0xBB, 0x00, 0x28, 0x00, 0x00, 0x28, 0x7E};
byte setBaudrate[] = {0xBB, 0x00, 0x11, 0x00, 0x02, 0x00, 0x60, 0x73, 0x7E};

String receive = "";
int receivex[100] = {};
int i = 0;


void printWelcomeMessage(void) {
  Serial.println("Choose from the Menue and enter the number:");
  Serial.println("1. Single Read");
  Serial.println("2. Multi Read (to stop, select '3')");
  Serial.println("3. Stop Read");
  Serial.println("5. Print Menue");
  Serial.println("6. BAUDRATE9600");
}


void setup() {
  Serial.begin(115200);
  Serial.println("Serial started");

  mySerial.begin(115200);


  printWelcomeMessage();
}



void loop() {
  //read commands from Serial Monitor
//  if (Serial.available())
//  {
//    char letter = Serial.read();
//    if (letter == '1') {
//      mySerial.write(readSingle, sizeof(readSingle));
//    }
//    if (letter == '2') {
//      mySerial.write(readMulti, sizeof(readMulti));
//    }
//    if (letter == '3') {
//      mySerial.write(stopRead, sizeof(stopRead));
//    }
//    if (letter == '4') {
//      mySerial.write(readSingle, sizeof(readSingle));
//    }
//    else if (letter == '5') {
//      printWelcomeMessage();
//    }
//    else if (letter == '6') {
//      mySerial.write(setBaudrate, sizeof(setBaudrate));
//    }
//  }
  
  
  //delay(500);
  
  String id = getRFIDData();
  Serial.println(id);
  
  delay(3000);
  
//print RFID reply
//  String idList = "";
//  while (mySerial.available()) {
//    int hexIn = mySerial.read();
//    idList += String(hexIn,HEX);
//    idList += ",";
//    
//    //Serial.print(",");
//    if(hexIn == 0x7E){
//      Serial.print(idList);
//      Serial.println();
//    }
//  }

}

String getRFIDData(void) {
//print RFID reply
  mySerial.listen();
  mySerial.write(readSingle, sizeof(readSingle));
  String idList = "";
  while (mySerial.available()) {
    int hexIn = mySerial.read();
    idList += String(hexIn,HEX);
    idList += ",";

    if (!mySerial.available()){
      return idList;
    }
//    if(hexIn == 0x7E){
//      //Serial.print(idList);    
//      //Serial.println();
//      return idList;
//    }
  }
}
