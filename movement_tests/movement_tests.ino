#include <AccelStepper.h>
#include <PrintStream.h>
#include <Vex.h>

#define s1Control 12
#define s2Control 13
#define IRpin 3

// Number of steps per output rotation
const int stepsPerRevolution = 200;

AccelStepper step1(AccelStepper::FULL4WIRE, 4, 5, 6, 7); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper step2(AccelStepper::FULL4WIRE, 8, 9, 10, 11); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

IRObstacle ir(IRpin);

// Robot
Vex Robot;

Adafruit_DCMotor *m1 = Robot.setMotor(1);
Adafruit_DCMotor *m2 = Robot.setMotor(2);

unsigned long past = 0;
bool ran = true;

bool setMode(bool b) {
  if (b) {
    digitalWrite(s1Control, HIGH);
    digitalWrite(s2Control, HIGH);
  } else {
    digitalWrite(s1Control, LOW);
    digitalWrite(s2Control, LOW);
  }
  return b;
}

void run() {
  setMode(true);
  step1.runSpeed();
  step2.runSpeed();
}

void stop() {
  speed(0);
  setMode(false);
}

void delayMotors(double timeSec) {
  unsigned long time = millis();
  // Serial << "Delaying for " << timeSec << " secs" << endl;
  while (millis() - time < (timeSec * 1000)) {
    run();
  } 
  stop();
}

// This function runs the motors continuously
void speed(double speed) {
  setMode(true);
  step1.setSpeed(-speed);
  step2.setSpeed(speed);
  if (speed == 0) return setMode(false);
  // Serial << "Stepping continuously with speed " << speed << endl;
}

// This function runs for the time and stops motors after
void speed(double speed, double timeSec) {
  unsigned long time = millis();
  // Serial << "Stepping with speed " << speed << " for " << timeSec << " secs" << endl;
  while (millis() - time < (timeSec * 1000)) {
    step1.setSpeed(-speed);
    step2.setSpeed(speed);
    run();
  }
  stop();
}

void homeSteppers() {
  while (!ir.isObstacle()) {
    speed(-50);
    run(); 
  }
  stop();
  delayMotors(1);
  speed(35);
  delayMotors(.5);
  delay(1000);
}


void setup() {

	Serial.begin(9600);

  Robot.begin();
  ir.begin();

  step1.setMaxSpeed(200);
  step1.setAcceleration(100);

  step2.setMaxSpeed(200);
  step2.setAcceleration(100);

  pinMode(s1Control, OUTPUT);
  pinMode(s2Control, OUTPUT);

  Serial << "Hey!! Lets go!" << endl;
  delay(5000);
  homeSteppers();
}

void loop() {
  // if (step1.speed() != 0 && step2.speed() != 0) {
  //   step1.runSpeed();
  //   step2.runSpeed();
  // }


  // Robot.moveMotor(m2, 100, 5);
  Robot.moveTank(m1, m2, 100, 100, 5);
  delay(1500);

  // VERSION 1: move cont. with certain time
  speed(stepsPerRevolution, 2);
  delayMotors(2);
  
  // // // Version 2: move cont. with delays
  speed(-stepsPerRevolution);
  delayMotors(2);
  delayMotors(3);
  // Serial << "================" << endl;
}
