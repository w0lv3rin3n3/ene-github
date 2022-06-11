#define MQ2pin 4

float sensorValue;  //variable to store sensor value

void setup()
{
  Serial.begin(115200); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");
  delay(5000); // allow the MQ-2 to warm up
}

void loop()
{
  sensorValue = analogRead(MQ2pin); // read analog input pin 0
  
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  
  if(sensorValue > 2900)
  {
    Serial.print(" | Smoke detected!");
  }
  
  Serial.println("");
  delay(2000); // wait 2s for next reading
}
