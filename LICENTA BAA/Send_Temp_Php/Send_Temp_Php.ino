#include <Connect_to_WiFi.hpp>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Domain name and URL path or IP address with path
const char* serverName = "http://licenta-smart-home.freecluster.eu/post-esp-data.php/";

// Keep this API Key value to be compatible with the PHP code.
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "DHT_11";
String sensorLocation = "Office";

// Digital pin connected to the DHT sensor
#define DHTPIN 4  

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  connect_to_wifi();

  dht.begin();
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&temperature=" + String(dht.readTemperature())
                          + "&humidity=" + String(dht.readHumidity()) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}
