#include <Stepper.h>

// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 

//  Amount of Gear Reduction
const float GEAR_RED = 64;

// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

// Number of Steps Required
int StepsRequired;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
}
 
void loop() {
  if(Serial.available())
  {
    // Pins entered in sequence 1-3-2-4 for proper step sequencing
    Stepper steppermotor(STEPS_PER_REV, 8, 9, 10, 11);
    if(Serial.read() == '1')
    {    
      Serial.println("Open curtain");
      StepsRequired  =  STEPS_PER_OUT_REV * 3.5;   
      steppermotor.setSpeed(600);  
      steppermotor.step(StepsRequired);  

    }
    else 
    {
      Serial.println("Close curtain");
      StepsRequired  = - STEPS_PER_OUT_REV * 3.5;   
      steppermotor.setSpeed(600);  
      steppermotor.step(StepsRequired);  
    }
  }
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
}
