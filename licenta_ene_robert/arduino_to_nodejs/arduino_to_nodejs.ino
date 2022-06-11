#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Digital pin connected to the DHT sensor
#define DHTPIN 4  

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {

    DynamicJsonDocument jBuffer(1024);
    float Temperature = dht.readTemperature();
    float Humidity = dht.readHumidity();
    jBuffer["temperature"] = Temperature;
    jBuffer["humidity"] = Humidity;
    serializeJson(jBuffer, Serial);
    Serial.println();

//    Serial.println(jBuffer);
  
  delay(5000);
  
}
