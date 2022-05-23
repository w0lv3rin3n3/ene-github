//#include <String.h>
#include <Connect_to_WiFi.hpp>

void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  dht.begin();              

  connect_to_wifi();
  
}
void loop() {
  
  server.handleClient();
  
}
