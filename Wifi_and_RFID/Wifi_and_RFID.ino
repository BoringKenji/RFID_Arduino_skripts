#include<SoftwareSerial.h>
SoftwareSerial esp(10, 9);  // RX, TX
SoftwareSerial uhf(5, 6);  // RX, TX

//link setting//fix in initWifi()
#define wifi_ssid "LSK 7028"//ap address
#define wifi_pwd "wesharetechnology"//ap password

//post setting
#define url "/hwinfo"
#define server "148.70.180.108"

//UHF Commands
byte readSingle[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
byte readMulti[] = {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0x00, 0x0A, 0x56, 0x7E};//10 times rotation
byte stopRead[] = {0xBB, 0x00, 0x28, 0x00, 0x00, 0x28, 0x7E};
byte setBaudrate[] = {0xBB, 0x00, 0x11, 0x00, 0x02, 0x00, 0x60, 0x73, 0x7E};

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("hardware serial!");
  esp.begin(9600);
  uhf.begin(115200);


  String cmd1 = "AT+CWJAP=\"";
  cmd1 += wifi_ssid;
  cmd1 += "\",\"";
  cmd1 += wifi_pwd;
  cmd1 += "\"";
  esp.println(cmd1);
  delay(5000);

  
  if (esp.find("OK")) {//获得连接ok
    esp.write("AT+CIFSR\r\n");//打IP地址
    Serial.println("ESP OK");
  } else {
    Serial.println("wifi reconnect");
  }//建立对远程服务器的TCP连接
//  String cmd2 = "AT+CIPSTART=\"TCP\",\"";
//  cmd2 += server;
//  cmd2 += "\",5000";
//  esp.println(cmd2);//start a TCP connection.
//  if (esp.find("OK")) {
//    Serial.println("server connected");
//  }

}

void loop() {
  
  String id = getRFIDData();
  Serial.println(id);

//  id = getRFIDData();
//  Serial.println(id);
  
  httpPost();
  
  delay(5000);

}

//fucntions


void httpPost () {//post data
  esp.listen();
  Serial.println("inpost");
  String cmd2 = "AT+CIPSTART=\"TCP\",\"";
  cmd2 += server;
  cmd2 += "\",5000";
  esp.println(cmd2);//start a TCP connection.
  if (esp.find("OK")) {
    Serial.println("server connected");
  }
  String data = "{\"Targetid\": 1, \"Timestamp\":\"00\", \"Weight\":20, \"Unitinfo\": 1}";
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
//  Serial.println(sendCmd);
  esp.println(postRequest.length() );
  delay(500);

   
  if (esp.find(">")) {
    Serial.println("Sending.."); esp.print(postRequest);
    if ( esp.find("SEND OK")) {
      Serial.println("Packet sent");
      while (!esp.available()){} //wait for ESP response
      while (esp.available()) {
        String tmpResp = esp.readString();
        Serial.println(tmpResp);
      }
      //       close the connection
//      esp.println("AT+CIPCLOSE");
    }
  }
}

String getRFIDData(void) {
//print RFID reply
  uhf.listen();
  Serial.println("read RFID tag");
  
  uhf.write(readSingle, sizeof(readSingle));
  String idList = "";

  while (!uhf.available()){} //wait for RFID response 
  delay(500); //additional dealy, since RFID string can be quite long and take a while to arrive
  while (uhf.available()) {
    int hexIn = uhf.read();
    idList += String(hexIn,HEX);
    idList += ",";
    
    if (!uhf.available()){//wait till last HEX message has arrived
      return idList;
    }
  }
}
