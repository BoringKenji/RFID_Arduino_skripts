/**
   lab IoT demo on Arduino Nano
   LMT 2021.5.19
   Function description:
   initWifi(): start wifi connection
   initCIP(): start CIPSEND
   httpPost(): POST data
   getWeightData(): get data of weight sensor
   getRFIDData(): get data of RFID sensor, buzzer control is under this function
   buildData(): build JSON data string
   reset(): ESP reset

   new update on 2021.5.19
   UHF RFID part
   change buzzer into LED light
*/



#include<SoftwareSerial.h>
#include "HX711.h"
#include <SPI.h>

//basic setting of hardware
const String unitinfo = "1";
//WEIGHT SENSOR
const int DT_PIN = 5;
const int SCK_PIN = 6;
//BUZZER
//const int BEEP = A7;

//UHF RFID setting
SoftwareSerial uhf(3, 2); //RX,TX//115200
//UHF Commands
byte readSingle[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
byte readMulti[] = {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0x00, 0x0A, 0x56, 0x7E};//10 times rotation
byte stopRead[] = {0xBB, 0x00, 0x28, 0x00, 0x00, 0x28, 0x7E};
byte setBaudrate[] = {0xBB, 0x00, 0x11, 0x00, 0x02, 0x00, 0x60, 0x73, 0x7E};

String idList = "";


SoftwareSerial esp(10, 9);  // RX, TX//9600
HX711 scale;

//link setting//fix in initWifi()
#define wifi_ssid "iottest"//ap address
#define wifi_pwd "11111111"//ap password

//post setting
#define url "/hwinfo"
#define server "148.70.180.108"

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("WeShare RFID reader is ready!");
  esp.begin(9600);
  Serial.println("wifi start");
  uhf.begin(115200);
  Serial.println("active RFID start");
  //  esp.write("AT\r\n");
  //  delay(2000);

  SPI.begin();

  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale();  // 開始取得比例參數
  scale.tare();//////////程序崩坏点！！！！！！！

  initWifi();
}

void loop() {
  //  串口读取互显，调试用
  //    if (esp.available())
  //      Serial.write(esp.read());
  //    if (Serial.available())
  //      esp.write(Serial.read());

  String weight = String(getWeightData());
  Serial.println("finweight");
  delay(100);
  String id = getRFIDData();
  Serial.println(id);
  Serial.println("finrfid");
  delay(100);
  String timestamp = "1";
  String data = buildData(id, timestamp, weight, unitinfo);

  httpPost(data);
  delay(2000);
}

float getWeightData() {
  delay(2000);
  float current_weight = scale.get_units(10);
  float true_weight = (current_weight) / -200;
  Serial.print("Weight: ");
  Serial.println((current_weight) / -200, 0.0);
  return true_weight;
}


String getRFIDData(void) {
//print RFID reply
  uhf.listen();
  uhf.write(readSingle, sizeof(readSingle));
  String idList = "";
  while (uhf.available()) {
    int hexIn = uhf.read();
    idList += String(hexIn,HEX);
    idList += ",";
    
    if (!uhf.available()){//wait till last HEX message has arrived
      return idList;
    }
  }
}

void initWifi() {
  esp.listen();
  delay(2000);
  Serial.println("initWifi");
  String cmd1 = "AT+CWJAP=\"";
  cmd1 += wifi_ssid;
  cmd1 += "\",\"";
  cmd1 += wifi_pwd;
  cmd1 += "\"";
  esp.println(cmd1);
  delay(5000);

  if (esp.find("OK")) {//获得连接ok
    Serial.println("wifi initialized.");
  } else {
    //    initWifi();
    Serial.println("wifi reconnect");
  }//建立对远程服务器的TCP连接
}

void initCIP() {
//  esp.listen();
  Serial.println("initCIP()");
  String cmd2 = "AT+CIPSTART=\"TCP\",\"";
  cmd2 += server;
  cmd2 += "\",5000";
  esp.println(cmd2);//start a TCP connection.
  if (esp.find("OK")) {
    Serial.println("server connected");
  }
}

void httpPost (String data) {//post data
  Serial.println("httpPost():" + data);
  initCIP();
  String postRequest = "POST ";
  postRequest += url;
  postRequest += " HTTP/1.0\r\n";
  postRequest += "Host: " ;
  postRequest += server;
  postRequest += "\r\n";
  postRequest += "Accept: *" ;
  postRequest += "/";
  postRequest += "*\r\n" ;
  postRequest += "Content-Length: " ;
  postRequest += data.length();
  postRequest += "\r\n" ;
  postRequest += "Content-Type: application/json\r\n" ;
  postRequest += "\r\n" ;
  postRequest += data;

  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  esp.print(sendCmd);
  esp.println(postRequest.length() );
  delay(500);
  if (esp.find(">")) {
    Serial.println("Sending..");
    esp.print(postRequest);
    if ( esp.find("SEND OK")) {
      Serial.println("Packet sent");
      while (esp.available()) {
        String tmpResp = esp.readString();
        Serial.println(tmpResp);
      }
      // close the connection
      //            esp.println("AT+CIPCLOSE");
    }
  }
}


String buildData(String id, String timestamp, String weight, String unitinfo) { //组建发送的json字符串
  String payload = "{";
  payload += "\"Targetid\":\"";
  payload += id;
  payload += "\",";
  payload += "\"Timestamp\":\"";
  payload += timestamp;
  payload += "\",";
  payload += "\"Weight\":";
  payload += weight;
  payload += ",";
  payload += "\"Unitinfo\":\"";
  payload += unitinfo;
  payload += "\"}";
  Serial.println(payload);
  return payload;
  //  char attributes[100];
  //  payload.toCharArray( attributes, 100 );
}


void reset() {//重置函数

  esp.println("AT+RST");

  delay(1000);

  if (esp.find("OK") ) Serial.println("Module Reset");

}

void array_to_string(byte array[], unsigned int len, char buffer[]) { //byte array to HEX string
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
