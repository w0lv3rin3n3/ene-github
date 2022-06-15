#define MQ2pin 2

float sensorValue;  //variable to store sensor value

void setup()
{
  Serial.begin(115200); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");
//  delay(5000); // allow the MQ-2 to warm up
}

void loop()
{
//  sensorValue = analogRead(MQ2pin); // read analog input pin 0

  //read data from MQ-135
  float co2comp = analogRead(MQ2pin);
  float co2ppm = map(co2comp,0,675,300,10000);
  
  Serial.print("Sensor Value: ");
  Serial.println(co2ppm);
  
//  if(sensorValue > 2900)
//  {
//    Serial.print(" | Smoke detected!");
//  }
  
//  Serial.println("");
  delay(2000); // wait 2s for next reading
}
