#include <Stepper.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 

//  Amount of Gear Reduction
const float GEAR_RED = 64;

// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

// Number of Steps Required
int StepsRequired;

// Pins entered in sequence 1-3-2-4 for proper step sequencing
Stepper steppermotor(STEPS_PER_REV, 36, 34, 39, 35);

String jsondata1;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  char* buff = "";
  jsondata1 = "";
  buff = (char*) incomingData;
  jsondata1 = String(buff);
  Serial.println(jsondata1);
  if(jsondata1 == "\"stepperState\":1")
  {
    Serial2.println("1");
    StepsRequired  =  STEPS_PER_OUT_REV * 2;   
    steppermotor.setSpeed(1000);  
    steppermotor.step(StepsRequired);    
  }
  else if(jsondata1 == "\"stepperState\":0")
  {
    Serial2.println("0");
    StepsRequired  = - STEPS_PER_OUT_REV * 2;   
    steppermotor.setSpeed(1000);  
    steppermotor.step(StepsRequired);    
  }
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
