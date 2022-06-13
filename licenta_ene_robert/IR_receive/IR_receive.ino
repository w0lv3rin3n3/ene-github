#include <IRremote.h>
#include <ir_SAMSUNG.hpp>

IRsend irsend = IRsend(3);
IRrecv irrecv(12);
decode_results results;

const int ledPin = 16;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(300);
  irrecv.enableIRIn();

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
//  IRsend irsend;
//  irsend.enableIROut();
}

void loop() {
  // put your main code here, to run repeatedly:
//  irsend.sendNEC(0xF4BA2988, 32);  
//  Serial.println("am trimis");
//  irsend.sendSamsung(0x707, 0x79, 1, false);
  while(!irrecv.decode()){}
//  Serial.println("while");
  if(irrecv.decode())
  {
//    Serial.println(, HEX);
    Serial.println("----------");
//    printIRResultShort(&Serial, &irrecv.decodedIRData);
//    irrecv.decodeLG();
//    Serial.println(irrecv.decodeLG());
    irrecv.resume();
  }
  delay(2000);
}
