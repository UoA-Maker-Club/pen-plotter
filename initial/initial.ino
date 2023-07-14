#include <Stepper.h>
#include <Servo.h>
#include <math.h>

class Joint {
public:
  Stepper stepper = Stepper(totalSteps, 0, 0, 0, 0);
  float currentAngle;
  float length;
  int totalSteps = 2048;

  Joint(float _length, int pin1, int pin2, int pin3, int pin4) {
    length = _length;
    currentAngle = PI;
    stepper = Stepper(totalSteps, pin1, pin2, pin3, pin4);
    stepper.setSpeed(5);
  }

  void moveStepper(float angle) {
    float deltaAngle = angle - currentAngle;
    float nSteps =  deltaAngle * (totalSteps / (2 * PI));
    stepper.step(nSteps);
    Serial.print("\n angleDelta :" + String(deltaAngle) + "   Steps: " + String(nSteps));
    int time = nSteps*5.87;
    delay(time);
    currentAngle += deltaAngle;  //updates the stepper angle to the new angle
  }
};

class Pen {
public:
  bool isDrawing;
  Servo penServo;

  Pen(int pin) {
    penServo.attach(pin);
  } 

  void raise() {
    penServo.write(90);
    isDrawing = false;
    delay(8);
  }

  void lower() {
    penServo.write(0);
    isDrawing = true;
    delay(8);
  }

  void swap() {
    if (isDrawing){
      raise();
    }
    else {
      lower();
    }
  }
};

float jointLength = 100;
Joint elbow = Joint(jointLength, 7, 8, 9, 10);
Joint shoulder = Joint(jointLength, 3, 4, 5, 6);
Pen pen = Pen(2);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // initialises the builtin LED
  int stepSize = 1; //the distance that the arm tries to step on its way to a
  Serial.begin(9600);
}

void loop() {


  drawRectangle(50,40,60,30); 
  
}

void drawLine(int startX, int startY, int finishX, int finishY){// takes the start and end point of a line and goes to a series of points along that line with the pen down
  pen.raise();
  pointTo(startX, startY);
  pen.lower();
  float slope = (finishY-startY)/(finishX - startX);
  for (int x = (10*startX); x <= (10*finishX); x++){ //if too slow may need to remove the factor of 10
    float y = slope*((x/10) - startX) - startY; // points on line y = m(x-x1)+y1
    pointTo(x,y);
  }
  pen.raise();
}

void drawRectangle(int top, int bottom, int left, int right) { //takes in the 4 extents of a rectangle and draws it
  drawLine (left,top , right,top);
  drawLine (right,top , right,bottom);
  drawLine (right,bottom , left,bottom);
  drawLine (left,bottom , left,top);

}

void pointTo(float x, float y) {  //takes in an xy coordinate and points the arm to it

  //step one is to convert to polar cordinates
  float radius = sqrt(sq(x) + sq(y));
  float angle = atan(y / x);

  //next determine the internal angles of a triangle made by connecting the shoulder elbow and hand (Cosine rule)
  float elbowAngle = acos((2 * sq(jointLength) - sq(radius)) / (2 * sq(jointLength)));
  float internalShoulderAngle = acos(sq(radius) / (2 * jointLength * radius));

  //process these internal angles into the actual joint angles
  float shoulderAngle = (angle - internalShoulderAngle + PI / 2);


  //write to the servos
  elbow.moveStepper(elbowAngle);
  shoulder.moveStepper(shoulderAngle);

}
