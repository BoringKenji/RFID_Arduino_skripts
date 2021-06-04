#include<SoftwareSerial.h>
SoftwareSerial esp(10, 9);  // RX, TX

//link setting//fix in initWifi()
//#define wifi_ssid "iottest"//ap address
//#define wifi_pwd "11111111"//ap password
#define wifi_ssid "Josef_Laptop"//ap address
#define wifi_pwd "12345678"//ap password

//post setting
#define url "/hwinfo"
#define server "148.70.180.108"


void setup() {
  Serial.begin(115200);
  Serial.println("hardware serial ready!");
  esp.begin(9600);
  reset_esp();
  //connectWifi();
}

void loop(){
  
}


//functions**********************************************************************8

void reset_esp() {
  esp.println("AT+RST");
  delay(1000);
  if(esp.find("OK") ) Serial.println("Module Reset");
}

void connectWifi() {
  String cmd1 = "AT+CWJAP=\"";
  cmd1 += wifi_ssid;
  cmd1 += "\",\"";
  cmd1 += wifi_pwd;
  cmd1 += "\"";
  esp.println(cmd1);  
  delay(4000);  
  if(esp.find("OK")) { 
    Serial.println("Connected!");
  }
  else {  
    connectWifi();  
    Serial.println("Cannot connect to wifi"); 
  }  
}

//  byte read_data () {
//
//  byte data;
//
//  for (int i = 0; i < 8; i ++) {
//
//    if (digitalRead (DHpin) == LOW) {
//  
//    while (digitalRead (DHpin) == LOW); // wait for 50us
//  
//    delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
//  
//    if (digitalRead (DHpin) == HIGH)
//  
//    data |= (1 << (7-i)); // high front and low in the post
//  
//    while (digitalRead (DHpin) == HIGH);
//  
//    // data '1 ', wait for the next one receiver
//        
//    }
//
//  } return data; 
