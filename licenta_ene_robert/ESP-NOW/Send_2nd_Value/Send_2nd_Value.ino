//#include <esp_now.h>
//#include <WiFi.h>
#include <ArduinoJson.h>
//#include <Wire.h>
#define LED 22
//
//uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x9D, 0x84, 0xE8};
//
//esp_now_peer_info_t peerInfo;
//
//// callback when data is sent
//void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//  Serial.print("\r\nLast Packet Send Status:\t");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//}

//#define SLAVE_ADDR 9
//#define ANSWERSIZE 5
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
//  Wire.begin(23, 19);
  pinMode(LED, OUTPUT);
 
  // Set device as a Wi-Fi Station
//  WiFi.mode(WIFI_STA);
//
//  // Init ESP-NOW
//  if (esp_now_init() != ESP_OK) {
//    Serial.println("Error initializing ESP-NOW");
//    return;
//  }
//
//  esp_now_register_send_cb(OnDataSent);
//  
//  // Register peer
//  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//  peerInfo.channel = 0;  
//  peerInfo.encrypt = false;
//  
//  // Add peer        
//  if (esp_now_add_peer(&peerInfo) != ESP_OK){
//    Serial.println("Failed to add peer");
//    return;
//  }
//   esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Set values to send
    DynamicJsonDocument jBuffer(1024);
    String jsondata1 = "";
    float Temperature = 100;
    float Humidity = 50;
    jBuffer["primul"] = Temperature;
    jBuffer["al_doilea"] = Humidity;
    serializeJson(jBuffer, jsondata1);
//    jsondata1 = "1@" + jsondata1;
//    Wire.beginTransmission(SLAVE_ADDR);
//    Wire.write(0);
//    Wire.endTransmission();

//    Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);
//    String responseString = "";
//    while(Wire.available()) {
//      char b = Wire.read();
//      responseString += b;
//    }
  // Send message via ESP-NOW
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) jsondata1.c_str(), jsondata1.length());
    Serial.println(jsondata1);

    if(Serial.available())
    {
      String inByte = Serial.readString();
      Serial.println(inByte);
      if(inByte == "1")
        digitalWrite(LED, LOW);
      else if(inByte == "0")
        digitalWrite(LED, HIGH);
    }
   
//  if (result == ESP_OK) {
//    Serial.println("Sent with success");
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
  delay(3000);
}
