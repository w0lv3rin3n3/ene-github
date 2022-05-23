#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Digital pin connected to the DHT sensor
#define DHTPIN 4  

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

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
  dht.begin();
}
 
void loop() {
  // Set values to send
    DynamicJsonDocument jBuffer(1024);
    String jsondata = "";
    float Temperature = dht.readTemperature();
    float Humidity = dht.readHumidity();
    jBuffer["temperature"] = Temperature;
    jBuffer["humidity"] = Humidity;
    serializeJson(jBuffer, jsondata);
    jsondata = "0@" + jsondata;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) jsondata.c_str(), jsondata.length());
  Serial.println(jsondata);
//  Serial.println("Primul Json:" + jsondata.length());
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(5000);
}
