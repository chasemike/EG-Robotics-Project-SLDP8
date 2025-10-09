#include <AccelStepper.h>
#include <PrintStream.h>
#include <Vex.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

AccelStepper step1(AccelStepper::FULL4WIRE, 4, 5, 6, 7); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper step2(AccelStepper::FULL4WIRE, 8, 9, 10, 11); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// Robot
Vex Robot;

Adafruit_DCMotor *m1 = Robot.setMotor(1);
Adafruit_DCMotor *m2 = Robot.setMotor(2);

unsigned long past = 0;
bool ran = true;

void run() {
  step1.runSpeed();
  step2.runSpeed();
}

void stop() {
  speed(0);
}

void delayMotors(unsigned timeSec) {
  unsigned long time = millis();
  Serial << "Delaying for " << timeSec << " secs" << endl;
  while (millis() - time < (timeSec * 1000)) {
    run();
  } 
  stop();
}

// This function runs the motors continuously
void speed(int speed) {
  step1.setSpeed(-speed);
  step2.setSpeed(speed);
  if (speed == 0) return;
  Serial << "Stepping continuously with speed " << speed << endl;
}

// This function runs for the time and stops motors after
void speed(int speed, int timeSec) {
  unsigned long time = millis();
  // Serial << "Stepping with speed " << speed << " for " << timeSec << " secs" << endl;
  while (millis() - time < (timeSec * 1000)) {
    step1.setSpeed(-speed);
    step2.setSpeed(speed);
    run();
  }
  stop();
}


void setup() {

	Serial.begin(9600);

  Robot.begin();

  step1.setMaxSpeed(200);
  step1.setAcceleration(100);

  step2.setMaxSpeed(200);
  step2.setAcceleration(100);

  Serial << "Hey!! Lets go!" << endl;
  delay(1000);
}

void loop() {
  if (step1.speed() != 0 && step2.speed() != 0) {
    step1.runSpeed();
    step2.runSpeed();
  }

  // Robot.moveMotor(m1, 100, 5);

  // VERSION 1: move cont. with certain time
  speed(stepsPerRevolution, 2);
  delayMotors(2);
  
  // // Version 2: move cont. with delays
  speed(-stepsPerRevolution);
  delayMotors(2);
  delayMotors(3);
  Serial << "================" << endl;
}
