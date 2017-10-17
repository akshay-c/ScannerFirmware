#include <Stepper.h>
#include <avr/interrupt.h>
#include <avr/io.h>

const int stepsPerRevolution = 2048;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8,10,9,11);
const int laserPin = 6;

int StepCount = 0;         // number of steps the motor has taken
int CurrentAngle = 0;
int RequestedAngle = 0;
char inChar = 'z';

void setup()
{
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(laserPin,OUTPUT);
  digitalWrite(laserPin,LOW);
  myStepper.setSpeed(5);
}

void loop()
{
  // step one step:
  if(inChar == 's' || inChar == 'S')
  {
    while(!Serial.available());
    RequestedAngle = (Serial.read()<<8);
    while(!Serial.available());
    RequestedAngle = RequestedAngle | Serial.read();
    CurrentAngle = CurrentAngle + RequestedAngle;
    unsigned long steps = (((unsigned long)CurrentAngle*(unsigned long)stepsPerRevolution)/360L);
    steps = steps - StepCount;
    myStepper.step(steps);
      StepCount+=steps;
    if(CurrentAngle >= 360)
    {
      CurrentAngle = CurrentAngle - 360;
      StepCount = StepCount - stepsPerRevolution;
    }
    delay(10);
    Serial.print((char)(RequestedAngle>>8));
    delay(10);
    Serial.print((char)RequestedAngle);
    delay(10);
//    Serial.print(RequestedAngle);
    Serial.print(inChar);
    inChar = 'z';
  }
  if(inChar == 'p' || inChar == 'P')
  {
      myStepper.step(stepsPerRevolution);
      delay(10);
      Serial.print(inChar);
    inChar = 'z';
  }
  if(inChar == 'F' || inChar == 'f')
  {
    digitalWrite(laserPin,LOW);
    delay(10);
    Serial.print(inChar);
    inChar = 'z';
  }
  if(inChar == 'l' || inChar == 'L')
  {
    digitalWrite(laserPin,HIGH);
    delay(10);
    Serial.print(inChar);
    inChar = 'z';
  }

}

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    inChar = (char)Serial.read(); 
  } 
}
