#include <Wire.h>

#define SLAVE_ADDR 9

#define ANSWERSIZE 5

String answer = "Hello";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
//  while(Serial.available())
//    Serial.println(Serial.readString());
  delay(3000);
}

void receiveEvent() {
  while (0 < Wire.available()) {
    byte x = Wire.read();
  }
  Serial.println("Receive event");
}

void requestEvent() {
  byte response[ANSWERSIZE];
  for (byte i = 0; i < ANSWERSIZE; i++)
    response[i] = (byte)answer.charAt(i);

    Wire.write(response, sizeof(response));
    Serial.println("Request event");
}
