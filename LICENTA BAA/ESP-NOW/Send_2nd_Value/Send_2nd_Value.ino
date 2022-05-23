#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x9D, 0x84, 0xE8};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
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
    jsondata1 = "1@" + jsondata1;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) jsondata1.c_str(), jsondata1.length());
  Serial.println(jsondata1);
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(5000);
}
