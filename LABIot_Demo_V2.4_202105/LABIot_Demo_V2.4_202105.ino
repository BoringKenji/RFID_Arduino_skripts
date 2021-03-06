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
const int DT_PIN = A0;
const int SCK_PIN = A1;
//BUZZER
//const int BEEP = A7;

//UHF RFID setting
SoftwareSerial uhf(5, 6); //RX,TX//115200
//UHF Commands
byte readSingle[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
byte readMulti[] = {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0x00, 0x0A, 0x56, 0x7E};//10 times rotation
byte stopRead[] = {0xBB, 0x00, 0x28, 0x00, 0x00, 0x28, 0x7E};
byte setBaudrate[] = {0xBB, 0x00, 0x11, 0x00, 0x02, 0x00, 0x60, 0x73, 0x7E};

String idList = "";


SoftwareSerial esp(3, 2);  // RX, TX//9600
HX711 scale;

//link setting//fix in initWifi()
#define wifi_ssid "LSK 7028"//ap address
#define wifi_pwd "wesharetechnology"//ap password

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

  //SPI.begin();

//  scale.begin(DT_PIN, SCK_PIN);
//  scale.set_scale();  // 開始取得比例參數
//  scale.tare();//////////程序崩坏点！！！！！！！

  //initWifi();
}

void loop() {

  //String weight = String(getWeightData());
  Serial.println("finweight");
  delay(100);
  char id [300] = getRFIDDataChar();
  Serial.println(id);


//  int id_len = id.length() + 1;
//  char str[id_len]; char rfid[6][100] = { 0 };
//  id.toCharArray(str, id_len);
//  Serial.println((String)"id: " + id);
//  int scan_length = sizeof(id);
//  Serial.println((String)"signal length: " + scan_length);
//  separate(id, rfid, scan_length);
//  
//  for (int j = 0; j < 6; j++) {
//    for (int i = 0; i < 100; i++) {
//      Serial.print(rfid[j][i]);
//    }
//
//    Serial.println();
//  }

//  //Separeat String
//  int id_len = id.length() + 1;
//  char str[id_len];
//  id.toCharArray(str, id_len);
//  char delim[] = " ";
//
//  char *ptr = strtok(str, delim);
//  
//  while(ptr != NULL)
//  {
//    Serial.println(ptr);
//    
//    ptr = strtok(NULL, delim);
//  }

//  Serial.println("finrfid");
//  delay(100);
//  String timestamp = "1";
//  String data = buildData(id, timestamp, weight, unitinfo);
//
//  httpPost(data);
  delay(2000);
}



//private fucntions *******************************************************


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
  Serial.println("read RFID tag");
  
  uhf.write(readSingle, sizeof(readSingle));
  String idList = "";

  while (!uhf.available()){} //wait for RFID response 
  delay(1000); //additional dealy, since RFID string can be quite long and take a while to arrive
  while (uhf.available()) {
    int hexIn = uhf.read();
    //if (String(hexIn).length() < 2) idList += "0"; //add a zero if hexIn is only one digit
    idList += String(hexIn,HEX);
    if (hexIn == 0x7E) idList += " ";
    else idList += ",";
    if (!uhf.available()){//wait till last HEX message has arrived
      return idList;
    }
  }
}


char getRFIDDataChar(void) {
//print RFID reply
  uhf.listen();
  Serial.println("read RFID tag");
  
  uhf.write(readSingle, sizeof(readSingle));
  char idList [300]= {0};

  while (!uhf.available()){} //wait for RFID response 
  delay(1000); //additional dealy, since RFID string can be quite long and take a while to arrive
  while (uhf.available()) {
    int hexIn = uhf.read();
    if (String(hexIn).length() < 2) idList += "0"; //add a zero if hexIn is only one digit
    idList += char(hexIn,HEX);
    if (hexIn == 0x7E) idList += char(" ");
    else idList += ",";
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
  esp.listen();
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
      while (!esp.available()){} //wait for ESP response
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

//1. seperate the different tags
// String id[0] = bb,2,22,0,d,c8,24,0,0,0,0,0,0,0,11,1d,79,20,e4,7e
void separate(char scan[], char store[][100], int lenght)
{
  int k = 0;
  for (int j = 0; j < 6; j++) {
    int i = 0;
    while (scan[i] != ' ')
    {
      store[j][i] = scan[k];
      i++; k++;
      if (k > lenght) {
        return;
      }
    }
    k++;
    
  }
  return;
}

// get id[5] eg c8 && get the 15th entry eg 1d
void getid(int idx, char rfid[][100], char id[][2])
{
  for(int a=0 ; a<6 ; a++)
  {
    if (rfid[a][0] != 'b')
    {
      return;
    }
    int i = 0, j = 0, k = 0;
    while (i < idx + 1)
    {

      if (i == idx)
      {
        id[a][k] = rfid[a][j];
        k++;
      }
      if (rfid[a][j] == ',')
      {
        i++;
      }
      j++;
    }
    for (int i = 0; i < 2; i++)
    {
      Serial.print(id[a][i]);
    }
    Serial.println();
  }
  return;
}
