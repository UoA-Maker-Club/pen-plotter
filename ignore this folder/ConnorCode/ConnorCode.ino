#include <AccelStepper.h>
#include <MultiStepper.h>

const int JOINT_LENGTH = 100;
const int MAX_SPEED = 1000;
const int STEPS = 2048;

typedef struct {
  double r;
  double theta;
} PPos;

typedef struct {
  int x;
  int y;

  PPos toPolar() {
    return {
      .r = sqrt(sq((double)x) + sq((double)y)),
      .theta = atan((double)x / (double)y)
    };
  }
} CPos;

int radToSteps(double radians) {
  return (radians / (2 * PI)) * STEPS;
}

class Arm {
public:
  Arm(AccelStepper elbew, AccelStepper shoulder)
    : m_elbow(elbew),
      m_shoulder(shoulder),
      m_prevElbowAngle(0),
      m_prevShoulderAngle(0),
      m_maxSpeed(MAX_SPEED) {

    m_elbow.setMaxSpeed(MAX_SPEED);
    m_shoulder.setMaxSpeed(MAX_SPEED);

    m_elbow.setSpeed(MAX_SPEED);
    m_shoulder.setSpeed(MAX_SPEED);


    m_elbow.setAcceleration(10000000);
    m_shoulder.setAcceleration(10000000000);
  }
  void MoveShoulderTo(float shoulderAngle){
    m_shoulder.moveTo(radToSteps(shoulderAngle));
    RunSteppers();
  }

  void LineTo(CPos pos) {
    PPos ppos = pos.toPolar();

    double elbowAngle = acos((2 * sq((double)JOINT_LENGTH) - sq(ppos.r)) / (2 * sq((double)JOINT_LENGTH)));
    double internalShoulderAngle = acos(sq(ppos.r) / (2 * JOINT_LENGTH * ppos.r));

    double shoulderAngle = ppos.theta - internalShoulderAngle + PI / 2;

    double dShoulderAngle = shoulderAngle - m_prevShoulderAngle;
    double dElbowAngle = elbowAngle - m_prevElbowAngle;

    double absDShoulderAngle = abs(dShoulderAngle);
    double absDElbewAngle = abs(dElbowAngle);

    int shoulderSign = dShoulderAngle < 0 ? -1 : 1;
    int elbewSign = dElbowAngle < 0 ? -1 : 1;

    Serial.println("~~~~~~  elbow sign: " + String(elbewSign) + "Shoulder Sign: " + String(shoulderSign) + " ~~~~~~~~~");

    m_shoulder.moveTo(radToSteps(shoulderAngle));
    m_elbow.moveTo(radToSteps(elbewSign));

    // if (absDShoulderAngle == absDElbewAngle) {
    //   Serial.println("IIIFFFF1");
    //   m_elbow.setSpeed(m_maxSpeed * elbewSign);
    //   m_shoulder.setSpeed(m_maxSpeed * shoulderSign);
    // } else if (absDElbewAngle > absDShoulderAngle) {

    //   float elbowSpeed = (m_maxSpeed * elbewSign);
    //   m_elbow.setSpeed(elbowSpeed);
      

    //   float shoulderSpeed = (m_maxSpeed * shoulderSign * (absDShoulderAngle / absDElbewAngle));
    //   m_shoulder.setSpeed(shoulderSpeed);
    //   Serial.println("IF2 shoulderSpeed = " + String(shoulderSpeed));

    // } else if (absDElbewAngle < absDShoulderAngle) {
      
    //   float elbowSpeed = (m_maxSpeed * elbewSign * (absDElbewAngle / absDShoulderAngle));
    //   m_elbow.setSpeed(elbowSpeed);
    //   Serial.println("IF3 elbowSpeed = " + String(elbowSpeed));

    //   float shoulderSpeed = (m_maxSpeed * shoulderSign);
    //   m_shoulder.setSpeed(shoulderSpeed);
    // }

    // Serial.print("Shoulder ");
    // Serial.print(absDShoulderAngle);
    // Serial.print("rad (");
    // Serial.print(radToSteps(shoulderAngle));
    // Serial.print(" steps) Speed ");
    // Serial.println(m_elbow.speed());


    // Serial.print("Elbow ");
    // Serial.print(absDElbewAngle);
    // Serial.print("rad (");
    // Serial.print(radToSteps(elbowAngle));
    // Serial.print(" steps) Speed ");
    // Serial.println(m_elbow.speed());
    // Serial.println("m_maxSpeed: " + String(m_maxSpeed));

    // RunSteppers();

//     m_prevElbowAngle = elbowAngle;
//     m_prevShoulderAngle = shoulderAngle;
//   }

// private:
//   AccelStepper m_elbow;
//   AccelStepper m_shoulder;

//   double m_prevShoulderAngle;
//   double m_prevElbowAngle;

//   int m_maxSpeed;

//   void RunSteppers() {
//     while ((m_elbow.distanceToGo() != 0) || (m_shoulder.distanceToGo() != 0)) {
//       //Serial.println("loop running");
//       if (m_elbow.distanceToGo() != 0){
//         m_elbow.runSpeed();
//       }


//       if (m_shoulder.distanceToGo() != 0){
//         m_shoulder.runSpeed();
//       }
//  //       if (m_shoulder.runSpeedToPosition()) Serial.println("Shoulder step: ");
//     }
//   }
// };

// AccelStepper shoulder(AccelStepper::FULL4WIRE, 3, 4, 5, 6);
// AccelStepper elbow(AccelStepper::FULL4WIRE, 7, 8, 9, 10);

// Arm arm(elbow, shoulder);

// void setup() {
//   shoulder.setSpeed(100);
//   Serial.println(shoulder.speed());

//   elbow.setAcceleration(30);
//   shoulder.setAcceleration(30);

//   Serial.begin(9600);

//   Serial.println("x30, y100");
//   arm.LineTo({ .x = 70, .y =30 });

//   Serial.println("x100, y30");
//   arm.LineTo({ .x = 100, .y =30 });

//   Serial.println("x30, y100");
//   arm.LineTo({ .x = 30, .y =100 });
//   // arm.MoveShoulderTo((-0.5*PI));

// }

// void loop() {}
