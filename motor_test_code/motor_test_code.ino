// #include <Stepper.h>
#include <AccelStepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

AccelStepper step1(AccelStepper::FULL4WIRE, 8, 9, 10, 11); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// Create Instance of Stepper library
// Stepper step1(stepsPerRevolution, 4, 5, 6, 7);
// Stepper step2(stepsPerRevolution, 8, 9, 10, 11);


void setup()
{
  step1.setMaxSpeed(200);
  step1.setAcceleration(100);
	// // set the speed at 60 rpm:
	// step1.setSpeed(200);
	// step2.setSpeed(200);
	// // initialize the serial port:
	Serial.begin(9600);
}

void move(int steps) {
  step1.move(steps);
	// step2.step(-steps);
}

void stop() {
  step1.stop();
	// step2.step(-steps);
}


unsigned long prev = 0;
void loop() 
{

  step1.run();
	// step one revolution in one direction:
	// Serial.println("clockwise");
	// move(stepsPerRevolution);

  unsigned long currentMillis = millis();
  Serial.println(currentMillis - prev);
  if (currentMillis - prev > 5000) {
    // prev = currentMillis;
    stop();
  } else if (currentMillis - prev > 2500) {
    // prev = currentMillis;
    move(200);
  }

	// step one revolution in the other direction:
	// Serial.println("counterclockwise");
  // move(-stepsPerRevolution);
	// delay(2500);
}