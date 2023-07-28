#include <AccelStepper.h>//loads an external library
#include <MultiStepper.h>//loads an external library
#include <Servo.h>//loads an external library
#define MotorInterfaceType 4//defines the type of stepper motor we are using
const int MAX_SPEED = 80;//dont mess with these
const int STEPS = 2048;//dont mess with these

//~~~~~~~~~~~JOINT LENGTHS~~~~~~~~~~~~~~~~~//
const int shoulderToElbow = 100;
const int elbowToPen = 120;//change this one based on the distance of your elbow to 
//~~~~~~~~~~SERVO ANGLES~~~~~~~~~~~~~//
const int drawAngle = 10;//change this one so your pen just touches the paper
const int moveAngle = 45;

typedef struct {
  double r;
  double theta;
} PPos;

typedef struct {
  int x;
  int y;

  void scaleMap() {
    x = map(x, 0, 150, 150, 30);
    y = map(y, 0, 150, 150, 30);
  }

  PPos toPolar() {
    return {
      .r = sqrt(sq((double)x) + sq((double)y)),
      .theta = atan((double)x / (double)y)
    };
  }
} CPos;

AccelStepper shoulder(AccelStepper::FULL4WIRE, 3, 5, 4, 6);// creates the shoulder stepper object and attaches it to pins 3,4,5,6
AccelStepper elbow(AccelStepper::FULL4WIRE, 7, 9, 8, 10);// creates the shoulder stepper object and attaches it to pins 7,8,9,10
Servo servo;


MultiStepper Arm;//creates the arm object

int radToSteps(double radians) {
  return (radians / (2 * PI)) * STEPS;
}

void RunSteppers() {

  while (Arm.run()) {
    delay(10);
  }
}

void goTo(CPos pos) {
  pos.scaleMap();
  PPos ppos = pos.toPolar();

  double elbowAngle = acos((sq(shoulderToElbow) + sq(elbowToPen) - sq(ppos.r)) / (sq(shoulderToElbow) + sq(elbowToPen)));
  double internalShoulderAngle = acos(sq(ppos.r) / ((elbowToPen + shoulderToElbow) * ppos.r));

  double shoulderAngle = ppos.theta - internalShoulderAngle + PI / 2;
  long steps[2] = { radToSteps(shoulderAngle), radToSteps(elbowAngle) };
  Arm.moveTo(steps);
  RunSteppers();
}

void drawLine(CPos previous, CPos pos) {
  int riseSign;
  int runSign;
  int rise = pos.y - previous.y;
  if (rise > 0){
    riseSign = 1;
  }
  else{
    riseSign = -1;
  }
  int run = pos.x - previous.x;
  if (run > 0){
    runSign = 1;
  }
  else{
    runSign = -1;
  }
  float gradient = abs(rise) / abs(run);
  servo.write(moveAngle);
  goTo(previous);
  servo.write(drawAngle);
  if (abs(rise) > abs(run)) {
    for (int i = 0; i < abs(rise); i++) {
      CPos point = {.x = (((int)(i / gradient))*runSign) + previous.x,.y = (i*riseSign) + previous.y};
      goTo(point);
    }
  }
  else if (abs(run) >= abs(rise)) {
    for (int i = 0; i < abs(run); i++) {
      CPos point = {.x = (i*runSign) + previous.x , .y = (((int)(i * gradient))*riseSign) + previous.y};
      goTo(point);
    }
  }
  servo.write(moveAngle);
}



void setup() {

//~~~~~~~INITIALISATION(don't mess with these)~~~~~~~//
  Serial.begin(9600);
  shoulder.setMaxSpeed(MAX_SPEED);//sets the max speed of the shoulder
  elbow.setMaxSpeed(MAX_SPEED);//sets the max speed of the elbow
  Arm.addStepper(shoulder);//adds the shoulder stepper to the arm object
  Arm.addStepper(elbow);//adds the elbow stepper to the arm object
  servo.attach(2);//attaches the servo to pin 2


//~~~~~~~~~~~~~~~DRAWING CODE~~~~~~~~~~~~~~~~//
//
  drawLine({.x = 100, .y = 100},{.x = 150, .y = 150});
  







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
}

void loop() {
//~~~~~~~~~~CODE HERE WILL RUN IN A LOOP FOREVER~~~~~~~~~~~~//


}
