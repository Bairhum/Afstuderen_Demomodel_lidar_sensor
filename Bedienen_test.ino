

// Pin assignments for buttons
#define Dance_pin  2
#define Reset_pin  3
#define Scan_pin  4

// Pin assignments for LED
#define LED_PIN 11
// Pin assignments for Azimuth motor
#define STEP_PIN_MOTOR_A 5
#define DIR_PIN_MOTOR_A 6
#define Enable_A_PIN 7

// Pin assignments for Elevation motor
#define STEP_PIN_MOTOR_E 8
#define DIR_PIN_MOTOR_E 9
#define Enable_E_PIN 10

void setup() {
 pinMode(Scan_pin, INPUT_PULLUP);
  pinMode(Reset_pin, INPUT_PULLUP);
  pinMode(Dance_pin, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(STEP_PIN_MOTOR_E, OUTPUT);
  pinMode(DIR_PIN_MOTOR_E, OUTPUT);
  pinMode(Enable_E_PIN, OUTPUT);
  pinMode(STEP_PIN_MOTOR_A, OUTPUT);
  pinMode(DIR_PIN_MOTOR_A, OUTPUT);
  pinMode(Enable_A_PIN, OUTPUT);

}

void loop() {

 if (digitalRead(Scan_pin)) {
    digitalWrite(LED_PIN, LOW);
  } 
  else if(digitalRead(Reset_pin) ) {
    digitalWrite(Enable_E_PIN, LOW);
    digitalWrite(Enable_A_PIN, LOW);
  }
    else if(digitalRead(Dance_pin) ) {
    digitalWrite(Enable_E_PIN, HIGH);
    digitalWrite(Enable_A_PIN, HIGH);
  }
  else if(digitalRead(Scan_pin) == LOW){
        digitalWrite(LED_PIN, HIGH);
  }
}
