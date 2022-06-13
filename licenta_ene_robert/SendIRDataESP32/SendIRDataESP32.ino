#include <IRremote.hpp>
#define DELAY_AFTER_SEND 4000

IRsend irsend = IRsend(16);

const int ledPin = 16;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

uint16_t sAddress = 0x0;
uint8_t ONCommand = 0x7;
uint8_t OFFCommand = 0x6;
uint8_t sRepeats = 1;

void sendIRData(uint8_t command) {
   IRData IRSendData;
    // prepare data
    IRSendData.address = sAddress;
    IRSendData.command = command;
    IRSendData.flags = IRDATA_FLAGS_EMPTY;

    IRSendData.protocol = NEC;
    Serial.print(F("Send "));
    Serial.println(getProtocolString(IRSendData.protocol));
    Serial.println(IRSendData.address, HEX);
    Serial.println(IRSendData.command, HEX);
    Serial.flush();
    irsend.write(&IRSendData, sRepeats);
}

void setup() {
  Serial.begin(115200);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendIRData(ONCommand);
  delay(DELAY_AFTER_SEND);
  sendIRData(OFFCommand);
  delay(DELAY_AFTER_SEND);

//  
//    IRData IRSendData;
//    // prepare data
//    IRSendData.address = sAddress;
//    IRSendData.command = ONCommand;
//    IRSendData.flags = IRDATA_FLAGS_EMPTY;
//
//    IRSendData.protocol = SAMSUNG;
//    Serial.print(F("Send "));
//    Serial.println(getProtocolString(IRSendData.protocol));
//    Serial.println(IRSendData.address, HEX);
//    Serial.println(IRSendData.command, HEX);
//    Serial.flush();
//    irsend.write(&IRSendData, sRepeats);
//    delay(DELAY_AFTER_SEND);
}
