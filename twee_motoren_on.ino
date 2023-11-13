const int  Dance_PIN = 9;    // first push button
const int  Scan_PIN = 7;    
const int ledPin1 = 6;       // LED

// Definieer de aansluitingen voor de eerste stappenmotor
#define STEP_PIN_MOTOR1 4
#define DIR_PIN_MOTOR1 5
#define LED_PIN 6 // Voeg LED-pin toe

// Definieer de aansluitingen voor de tweede stappenmotor
#define STEP_PIN_MOTOR2 2
#define DIR_PIN_MOTOR2 3

const int stepsPerRevolution = 400;
const int maxSpeed = 2*stepsPerRevolution;


 
void setup() 
{
  pinMode(Dance_PIN, INPUT);
  pinMode(Scan_PIN, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(STEP_PIN_MOTOR1, OUTPUT);
  pinMode(DIR_PIN_MOTOR1, OUTPUT);
  pinMode(STEP_PIN_MOTOR2, OUTPUT);
  pinMode(DIR_PIN_MOTOR2, OUTPUT);
  pinMode(LED_PIN, OUTPUT); // Configureer de LED-pin als uitvoer

  digitalWrite(DIR_PIN_MOTOR1, HIGH);
  digitalWrite(DIR_PIN_MOTOR2, HIGH);
}
 
void loop() 
{
  bool DanceState = 0;
  bool ResetState = 0;
  bool ScanState = 0;

  // read the pushbutton input pin:
  DanceState = digitalRead(Dance_PIN);
  ScanState = digitalRead(Scan_PIN);
 

  int targetAnglesA_Dance[] = {-360, 180, -180, 360, -180, 180};
  int targetAnglesE_Dance[] = {-360, 1500, -90, 360, -1500, 90};
  int targetAnglesA_Scan[] = {-90, 360, -90, 180, -90, 360, -630};
  int targetAnglesE_Scan[] = {45, -45, 180, -90, 180, -45, -1400};
  
  while(DanceState) 
  {
    for (int i = 0; i < sizeof(targetAnglesA_Dance) / sizeof(targetAnglesA_Dance[0]); i++) {
      long targetPositionMotor1 = (long)targetAnglesA_Dance[i] * stepsPerRevolution / 360;
      long targetPositionMotor2 = (long)targetAnglesE_Dance[i] * stepsPerRevolution / 360;

      if (targetPositionMotor1 < 0) {
        digitalWrite(DIR_PIN_MOTOR1, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR1, HIGH);
      }

      if (targetPositionMotor2 < 0) {
        digitalWrite(DIR_PIN_MOTOR2, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR2, HIGH);
      }

      long stepsToMoveMotor1 = abs(targetPositionMotor1 );
      long stepsToMoveMotor2 = abs(targetPositionMotor2 );

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
    return; 
  }


  while(ScanState) 
  {
    for (int i = 0; i < sizeof(targetAnglesA_Scan) / sizeof(targetAnglesA_Scan[0]); i++) {
      long targetPositionMotor1 = (long)targetAnglesA_Scan[i] * stepsPerRevolution / 360;
      long targetPositionMotor2 = (long)targetAnglesE_Scan[i] * stepsPerRevolution / 360;

      if (targetPositionMotor1 < 0) {
        digitalWrite(DIR_PIN_MOTOR1, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR1, HIGH);
      }

      if (targetPositionMotor2 < 0) {
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

      // Laat de LED op pin 6 knipperen met 10 Hz gedurende 1 seconde
      for (int k = 0; k < 5; k++) {
        digitalWrite(LED_PIN, HIGH); // Schakel de LED aan
        delay(200); // Wacht 50 ms (half van de periode)
        digitalWrite(LED_PIN, LOW); // Schakel de LED uit
        delay(200); // Wacht nog eens 50 ms
      }
    } 
    return; 
  }


    delay(10);
}