#include <IRremote.h>

int IRpin = 12;
int button = 4;
//int LED = 3;
char buttonState = LOW;

IRrecv irrecv(IRpin);
decode_results results;
IRsend irsend;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(button, INPUT);
//  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //if (irrecv.decode(&results)) 
    //{
     //Serial.println(results.value, HEX); // Print the Serial 'results.value'
     // irrecv.resume();   // Receive the next value
    //}
  //buttonState = digitalRead(button);
  //if(buttonState == HIGH) {
    //Serial.println("Button pressed, sending data");
    //digitalWrite(LED, HIGH);
//    for(int i = 0; i < 3; i++) {
      irsend.sendNEC(0xFFE01FED, 32);
//      delay(40);
//    }
    delay(1000);
//  }
//  else {
//    digitalWrite(LED, LOW);
//  }
}
