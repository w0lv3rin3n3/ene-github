#include <IRremote.hpp>
#define DELAY_AFTER_SEND 2000

IRsend irsend = IRsend(4);

const int ledPin = 16;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

//uint16_t sAddress = 0x707;
//uint8_t sCommand = 0x79;
//uint8_t sRepeats = 1;

void setup() {
  Serial.begin(115200);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  // put your main code here, to run repeatedly:
    IRData IRSendData;
    // prepare data
    IRSendData.address = sAddress;
    IRSendData.command = sCommand;
    IRSendData.flags = IRDATA_FLAGS_EMPTY;

    IRSendData.protocol = SAMSUNG;
    Serial.print(F("Send "));
    Serial.println(getProtocolString(IRSendData.protocol));
    Serial.println(IRSendData.address, HEX);
    Serial.println(IRSendData.command, HEX);
    Serial.flush();
    irsend.write(&IRSendData, sRepeats);
    delay(DELAY_AFTER_SEND);
}
