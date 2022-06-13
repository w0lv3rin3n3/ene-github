#include <IRremote.h>
#include <ArduinoJson.h>

IRrecv irrecv = IRrecv(12);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("loop");
  while(!irrecv.decode()){}
//  Serial.println("while");
  if(irrecv.decode())
  {
//    Serial.println("-----------------------------");
//    printIRResultShort(&Serial, &irrecv.decodedIRData);
    DynamicJsonDocument jBuffer(512);
    String jsondata = "";
    jBuffer["sensor_name"] = "ir";
    jBuffer["command_name"] = "flash";
    jBuffer["protocol"] = "NEC";
    jBuffer["address"] = irrecv.decodedIRData.address;
    jBuffer["command_code"] = irrecv.decodedIRData.command;
//    Serial.println("=============================");
    serializeJson(jBuffer, jsondata);
    Serial.println(jsondata);
//    Serial.println("/////////////////////////////");
    irrecv.resume();
  }
}
