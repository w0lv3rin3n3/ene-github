#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 4  
#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x9D, 0x84, 0xE8};

#define mS_TO_S_FACTOR 1000
RTC_DATA_ATTR int ms;
RTC_DATA_ATTR int bootCount = 0;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
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
  if(bootCount == 0)
  {
    Serial.println("Waiting for sleeping period...");
    while (Serial.available() == 0){}
    ms = Serial.parseInt();
  }
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_timer_wakeup(ms * mS_TO_S_FACTOR);
  dht.begin();
}

// the loop function runs over and over again forever
void loop() {
    DynamicJsonDocument jBuffer(1024);
    String jsondata = "";
    float Temperature = dht.readTemperature();
    float Humidity = dht.readHumidity();
    jBuffer["temperature"] = Temperature;
    jBuffer["humidity"] = Humidity;
    serializeJson(jBuffer, jsondata);  
    Serial.println(jsondata);
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) jsondata.c_str(), jsondata.length());
    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    Serial.println("Going to sleep...");
    esp_deep_sleep_start();
}
