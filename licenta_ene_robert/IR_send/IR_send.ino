#include <IRremote.h>

IRrecv irrecv = IRrecv(12);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!irrecv.decode()){}
  if(irrecv.decode())
  {
    Serial.println("-----------------------------");
    printIRResultShort(&Serial, &irrecv.decodedIRData);
    Serial.println("=============================");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
    Serial.println("/////////////////////////////");
    irrecv.resume();
  }
}
