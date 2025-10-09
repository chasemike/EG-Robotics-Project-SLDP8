// #include <Stepper.h>
#include <AccelStepper.h>
#include <PrintStream.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

AccelStepper step1(AccelStepper::FULL4WIRE, 4, 5, 6, 7); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper step2(AccelStepper::FULL4WIRE, 8, 9, 10, 11); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
unsigned long past = 0;

// Create Instance of Stepper library
// Stepper step1(stepsPerRevolution, 4, 5, 6, 7);
// Stepper step2(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
  // EDITION 1: Stepper Lib
	// // set the speed at 60 rpm:
	// step1.setSpeed(200);
	// step2.setSpeed(200);
	// // initialize the serial port:


  step1.setMaxSpeed(200);
  step1.setAcceleration(100);

  step2.setMaxSpeed(200);
  step2.setAcceleration(100);

	Serial.begin(9600);
  Serial << "Hey!! Lets go!" << endl;
  delay(500);
}

void move(int steps) {
  step1.move(steps);
	step2.move(-steps);
}

void stop() {
  speed(0);
}

bool ran = true;

void speed(int speed) {
  step1.setSpeed(speed);
  step2.setSpeed(-speed);
}


void loop() {
  
  // EDITION 1: With Libary Stepper
	// step one revolution in one direction:
	// Serial.println("clockwise");
	// move(stepsPerRevolution);

	// step one revolution in the other direction:
	// Serial.println("counterclockwise");
  // move(-stepsPerRevolution);
	// delay(2500);

  // =================================================================

  // Edition 2: With Lib AccelStepper but with move function
  // step1.run();
  // step2.run();
  // unsigned long now = millis();
  // unsigned long elapsed = now - past;
  // Serial.print(elapsed);
  // Serial.print(" ");
  // Serial.println(ran);
  
  // if (elapsed > 0 && ran) {
  //   past = now;
  //   ran = false;
  //   move(200)
  // } else if (elapsed > 2500) {
  //   stop();
  //   past = now;
  //   ran = true;
  // }

  // =================================================================

  // EDITION 3: With Lib AccelStepper but with setSpeed function
  if (step1.speed() != 0 && step2.speed() != 0) {
    step1.runSpeed();
    step2.runSpeed();
  }

  unsigned long now = millis();
  unsigned long elapsed = now - past;
  // Serial << "Elasped: " << elapsed << " " << "ran: " << ran << " ";
  // Serial << "speed1: " << step1.speed() << " " << "speed2: " << step2.speed() << " " << endl;

  if (elapsed > 0 && ran) {
    past = now;
    ran = false;
    speed(stepsPerRevolution);
  } else if (elapsed > 2500) {
    stop();
    past = now;
    // ran = true;
  }


}