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
  mySerial.println("Hello, world?");

  // Setting Single Read Mode
  // command: BB 00 22 00 00 22 7E

  //BB 00 27 00 03 22 FF FF 4A 7E

//  byte message[] = {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0xFF, 0xFF, 0x4A, 0x7E};
//  mySerial.write(message, sizeof(message));

 
  mySerial.write(0xBB);  //Header
  mySerial.write(0xFF);  //Reserved
  mySerial.write(0x22);  //Length (Command + Data)
  mySerial.write(0xFF);  //Command (0x87 sets auto mode behavior
  mySerial.write(0x01);  //Data 1: Enable Auto-Read
  mySerial.write(0x22);  //Data 2: Mode – Parity decoded – Manchester RF/64
  mySerial.write(0x7E);  //Data 3: Total number of block to be read (2)
  
  delay(500);
  while(mySerial.available()>0)
  {
    mySerial.read();
  }
  Serial.println();
  Serial.println("RFID module started in Single Read Mode, Waiting for Card...");
}

void loop() {
  val = mySerial.read();
   while (val != 0xBB)
   {  //On Successful read, first byte will always be 0xFF
      val = mySerial.read();
      delay(1000);
   }

   Serial.read();              // reserved
   Serial.read();              // length
   Serial.read();              // command (indicates tag data)
   data[0] = Serial.read();    // we read data 1
   data[1] = Serial.read();    // we read data 2
   data[2] = Serial.read();    // we read data 2

   Serial.println(data[0]);
}
