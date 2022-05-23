#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

DynamicJsonDocument jBuffer(1024);
String jsondata;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  char* buff = "";
  jsondata = "";
  buff = (char*) incomingData;
  jsondata = String(buff);
  Serial.println(jsondata);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
