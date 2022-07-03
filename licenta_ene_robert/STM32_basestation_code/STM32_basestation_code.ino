#include <SoftwareSerial.h>

SoftwareSerial mySerial(PA10, PA9);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
}
 
void loop() {
  if(Serial.available())
  {
      Serial.println(Serial.readString());
  }
}
