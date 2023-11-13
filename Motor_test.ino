// Definieer de aansluitingen voor de eerste stappenmotor
#define STEP_PIN_MOTOR1 4
#define DIR_PIN_MOTOR1 5
#define DANS_PIN 7

// Definieer de aansluitingen voor de tweede stappenmotor
#define STEP_PIN_MOTOR2 2
#define DIR_PIN_MOTOR2 3


const int stepsPerRevolution = 200;
const int maxSpeed = 10 * stepsPerRevolution;

void setup() {
  pinMode(STEP_PIN_MOTOR1, OUTPUT);
  pinMode(DIR_PIN_MOTOR1, OUTPUT);
  pinMode(STEP_PIN_MOTOR2, OUTPUT);
  pinMode(DIR_PIN_MOTOR2, OUTPUT);
  

  digitalWrite(DIR_PIN_MOTOR1, HIGH);
  digitalWrite(DIR_PIN_MOTOR2, HIGH);
}

void loop() {


  int targetAnglesMotor1[] = {-360, 360, -360, 360, -360, 360,};
  int targetAnglesMotor2[] = {-360, 360, -360, 360, -360, 360,};

  for (int i = 0; i < sizeof(targetAnglesMotor1) / sizeof(targetAnglesMotor1[0]); i++) {
    long targetPositionMotor1 = (long)targetAnglesMotor1[i] * stepsPerRevolution / 360;
    long targetPositionMotor2 = (long)targetAnglesMotor2[i] * stepsPerRevolution / 360;

    if (targetPositionMotor1 < 0 ) {
      digitalWrite(DIR_PIN_MOTOR1, LOW);
    } else {
      digitalWrite(DIR_PIN_MOTOR1, HIGH);
    }

    if (targetPositionMotor2 < 0 ) {
      digitalWrite(DIR_PIN_MOTOR2, LOW);
    } else {
      digitalWrite(DIR_PIN_MOTOR2, HIGH);
    }

    long stepsToMoveMotor1 = abs(targetPositionMotor1);
    long stepsToMoveMotor2 = abs(targetPositionMotor2);

        for (long j = 0; j < max(stepsToMoveMotor1, stepsToMoveMotor2); j++) {
      if (j < stepsToMoveMotor1) {
        digitalWrite(STEP_PIN_MOTOR1, HIGH);
      }
      if (j < stepsToMoveMotor2) {
        digitalWrite(STEP_PIN_MOTOR2, HIGH);
      }
      delayMicroseconds(1000000 / maxSpeed);
      digitalWrite(STEP_PIN_MOTOR1, LOW);
      digitalWrite(STEP_PIN_MOTOR2, LOW);
      delayMicroseconds(1000000 / maxSpeed);
    }
  }
}
