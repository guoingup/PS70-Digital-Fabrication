#include <AccelStepper.h>

AccelStepper stepper1(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

void setup() {
  stepper1.setMaxSpeed(50);
  stepper1.setAcceleration(50);
}

void loop() {

  int position1 = 500;
  stepper1.moveTo(position1);
  stepper1.runToPosition(); 
  delay(5000);

  // Move to a different coordinate
  position1 = -100;
  stepper1.moveTo(position1);
  stepper1.runToPosition(); 
  delay(5000);
}
