// Variables to track the state of different actions
bool ScanStateOnce = false;
bool DanceStateOnce = false;
bool ScanStateInf = false;
bool DanceStateInf = false;
bool resetPressed = false;

// Variables to store the last press times for debounce
unsigned long lastScanPressTime = 0;
unsigned long lastDancePressTime = 0;
unsigned long lastResetPressTime = 0;

// Pin assignments for buttons
#define Dance_pin  2
#define Reset_pin  3
#define Scan_pin  4

// Pin assignments for Azimuth motor
#define STEP_PIN_MOTOR_A 5
#define DIR_PIN_MOTOR_A 6
#define Enable_A_PIN 7

// Pin assignments for Elevation motor
#define STEP_PIN_MOTOR_E 8
#define DIR_PIN_MOTOR_E 9
#define Enable_E_PIN 10

// Pin assignments for LED/Laser
#define LED_PIN 11

// Maximum speed for motor movement max 10 RPM, 200 steps/revolution, gearratio 1:4
// (10*200*4)/60= 1333 mircoseconds between the steps. 
const int maxSpeed = 1333;

// Current positions of motors
long currentPositionMotorE = 0;
long currentPositionMotorA = 0;

// Debounce delay 
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  pinMode(Scan_pin, INPUT);
  pinMode(Reset_pin, INPUT);
  pinMode(Dance_pin, INPUT);
  pinMode(STEP_PIN_MOTOR_E, OUTPUT);
  pinMode(DIR_PIN_MOTOR_E, OUTPUT);
  pinMode(Enable_E_PIN, OUTPUT);
  pinMode(STEP_PIN_MOTOR_A, OUTPUT);
  pinMode(DIR_PIN_MOTOR_A, OUTPUT);
  pinMode(Enable_A_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(DIR_PIN_MOTOR_E, HIGH);
  digitalWrite(DIR_PIN_MOTOR_A, HIGH);
}

void loop() {
  // Disable motor drivers
  digitalWrite(Enable_E_PIN, HIGH);
  digitalWrite(Enable_A_PIN, HIGH);

  // Arrays defining target positions for dance and scan actions in Azimuth and Elevation
  int targetPositionsA_Dance[] = {-10, 700, -90, 180, -910, 0};
  int targetPositionsE_Dance[] = {20, 360, -90, 180, -150, 0};
  int targetPositionsA_Scan[] = {20, 150, -90, 180, -150, -180, 0};
  int targetPositionsE_Scan[] = {180, -40, 90, -45, 90, -40, 0};

  if (digitalRead(Scan_pin) == HIGH) {
    if ((millis() - lastScanPressTime) > debounceDelay) {
      lastScanPressTime = millis();

      // Check the duration of the button press
      unsigned long startTime = millis();
      while (digitalRead(Scan_pin) == HIGH) {
        delay(10);
      }
      unsigned long duration = millis() - startTime;

      if (duration < 800) {
        // Short press (< 0,8 seconds)
        if (!ScanStateOnce) {
          Serial.println("Button 7 pressed. Starting Function 1.");
          ScanStateOnce = true;
        }
      } else {
        // Long press (>= 0,8 seconds)
        if (!ScanStateInf) {
          Serial.println("Button 7 pressed for 5 seconds or more. Starting Function 2.");
          ScanStateInf = true;
        }
      }
    }
  } else if (digitalRead(Dance_pin) == HIGH) {
    if ((millis() - lastDancePressTime) > debounceDelay) {
      lastDancePressTime = millis();

      // Check the duration of the button press
      unsigned long startTime = millis();
      while (digitalRead(Dance_pin) == HIGH) {
        delay(10);
      }
      unsigned long duration = millis() - startTime;

      if (duration < 800) {
        // Short press (< 0,8 seconds)
        if (!DanceStateOnce) {
          Serial.println("Button 9 pressed. Starting Function 1.");
          DanceStateOnce = true;
        }
      } else {
        // Long press (>= 0,8 seconds)
        if (!DanceStateInf) {
          Serial.println("Button 9 pressed for 5 seconds or more. Starting Function 2.");
          DanceStateInf = true;
        }
      }
    }
  } else if (digitalRead(Reset_pin) == HIGH && (millis() - lastResetPressTime) > debounceDelay && DanceStateOnce || DanceStateInf || ScanStateOnce || ScanStateInf) {
    lastResetPressTime = millis();
    Serial.println("Button 8 pressed. Stopping Function 1.");
    ScanStateOnce = false;
    ScanStateInf = false;
    Serial.println("Button 8 pressed. Stopping Function 2.");
    DanceStateOnce = false;
    DanceStateInf = false;
    resetPressed = true;
    delay(debounceDelay);
  }

  // Dance routine
  while (DanceStateOnce) {
    // Enable motor drivers
    digitalWrite(Enable_E_PIN, LOW);
    digitalWrite(Enable_A_PIN, LOW);
    delay(10);

    // Iterate through dance target positions
    for (int i = 0; i < sizeof(targetPositionsA_Dance) / sizeof(targetPositionsA_Dance[0]); i++) {
      long targetPositionMotorA = targetPositionsA_Dance[i];
      long targetPositionMotorE = targetPositionsE_Dance[i];

      // Set motor directions based on target positions
      if (targetPositionMotorA < currentPositionMotorA) {
        digitalWrite(DIR_PIN_MOTOR_A, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_A, HIGH);
      }
      if (targetPositionMotorE < currentPositionMotorE) {
        digitalWrite(DIR_PIN_MOTOR_E, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_E, HIGH);
      }

      // Check for reset button press to exit the loop
      if (digitalRead(Reset_pin) == HIGH) {
        break;
      }

      // Calculate steps to move for each motor
      long stepsToMoveMotorA = abs(targetPositionMotorA - currentPositionMotorA);
      long stepsToMoveMotorE = abs(targetPositionMotorE - currentPositionMotorE);

      // Move motors
      for (long j = 0; j < max(stepsToMoveMotorA, stepsToMoveMotorE); j++) {
        if (j < stepsToMoveMotorA) {
          digitalWrite(STEP_PIN_MOTOR_A, HIGH);
          currentPositionMotorA += (targetPositionMotorA < currentPositionMotorA) ? -1 : 1;
        }
        if (j < stepsToMoveMotorE) {
          digitalWrite(STEP_PIN_MOTOR_E, HIGH);
          currentPositionMotorE += (targetPositionMotorE < currentPositionMotorE) ? -1 : 1;
        }

        // Check for reset button press to exit the loop
        if (digitalRead(Reset_pin) == HIGH) {
          break;
        }

        delayMicroseconds(maxSpeed);
        digitalWrite(STEP_PIN_MOTOR_A, LOW);
        digitalWrite(STEP_PIN_MOTOR_E, LOW);
        delayMicroseconds(maxSpeed);
      }
    }

    // Disable motor drivers
    digitalWrite(Enable_E_PIN, HIGH);
    digitalWrite(Enable_A_PIN, HIGH);

    // Check for reset button press to exit the loop
    if (digitalRead(Reset_pin) == HIGH) {
      break;
    }
    DanceStateOnce = 0; 
    return;
  }

  // Dance routine
  while (DanceStateInf) {
    // Enable motor drivers
    digitalWrite(Enable_E_PIN, LOW);
    digitalWrite(Enable_A_PIN, LOW);
    delay(10);

    // Iterate through dance target positions
    for (int i = 0; i < sizeof(targetPositionsA_Dance) / sizeof(targetPositionsA_Dance[0]); i++) {
      long targetPositionMotorA = targetPositionsA_Dance[i];
      long targetPositionMotorE = targetPositionsE_Dance[i];

      // Set motor directions based on target positions
      if (targetPositionMotorA < currentPositionMotorA) {
        digitalWrite(DIR_PIN_MOTOR_A, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_A, HIGH);
      }
      if (targetPositionMotorE < currentPositionMotorE) {
        digitalWrite(DIR_PIN_MOTOR_E, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_E, HIGH);
      }

      // Check for reset button press to exit the loop
      if (digitalRead(Reset_pin) == HIGH) {
        break;
      }

      // Calculate steps to move for each motor
      long stepsToMoveMotorA = abs(targetPositionMotorA - currentPositionMotorA);
      long stepsToMoveMotorE = abs(targetPositionMotorE - currentPositionMotorE);

      // Move motors
      for (long j = 0; j < max(stepsToMoveMotorA, stepsToMoveMotorE); j++) {
        if (j < stepsToMoveMotorA) {
          digitalWrite(STEP_PIN_MOTOR_A, HIGH);
          currentPositionMotorA += (targetPositionMotorA < currentPositionMotorA) ? -1 : 1;
        }
        if (j < stepsToMoveMotorE) {
          digitalWrite(STEP_PIN_MOTOR_E, HIGH);
          currentPositionMotorE += (targetPositionMotorE < currentPositionMotorE) ? -1 : 1;
        }

        // Check for reset button press to exit the loop
        if (digitalRead(Reset_pin) == HIGH) {
          break;
        }

        delayMicroseconds(maxSpeed);
        digitalWrite(STEP_PIN_MOTOR_A, LOW);
        digitalWrite(STEP_PIN_MOTOR_E, LOW);
        delayMicroseconds(maxSpeed);
      }
    }

    // Disable motor drivers
    digitalWrite(Enable_E_PIN, HIGH);
    digitalWrite(Enable_A_PIN, HIGH);

    // Check for reset button press to exit the loop
    if (digitalRead(Reset_pin) == HIGH) {
      break;
    }
  }

    // Scan routine
  while (ScanStateInf) {
    // Enable motor drivers
    digitalWrite(Enable_E_PIN, LOW);
    digitalWrite(Enable_A_PIN, LOW);
    delay(10);

    // Iterate through scan target positions
    for (int i = 0; i < sizeof(targetPositionsA_Scan) / sizeof(targetPositionsA_Scan[0]); i++) {
      long targetPositionMotorA = targetPositionsA_Scan[i];
      long targetPositionMotorE = targetPositionsE_Scan[i];

      // Set motor directions based on target positions
      if (targetPositionMotorA < currentPositionMotorA) {
        digitalWrite(DIR_PIN_MOTOR_A, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_A, HIGH);
      }
      if (targetPositionMotorE < currentPositionMotorE) {
        digitalWrite(DIR_PIN_MOTOR_E, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_E, HIGH);
      }

      // Check for reset button press to exit the loop
      if (digitalRead(Reset_pin) == HIGH) {
        break;
      }

      // Calculate steps to move for each motor
      long stepsToMoveMotorA = abs(targetPositionMotorA - currentPositionMotorA);
      long stepsToMoveMotorE = abs(targetPositionMotorE - currentPositionMotorE);

      // Move motors
      for (long j = 0; j < max(stepsToMoveMotorA, stepsToMoveMotorE); j++) {
        if (j < stepsToMoveMotorA) {
          digitalWrite(STEP_PIN_MOTOR_A, HIGH);
          currentPositionMotorA += (targetPositionMotorA < currentPositionMotorA) ? -1 : 1;
        }
        if (j < stepsToMoveMotorE) {
          digitalWrite(STEP_PIN_MOTOR_E, HIGH);
          currentPositionMotorE += (targetPositionMotorE < currentPositionMotorE) ? -1 : 1;
        }

        // Check for reset button press to exit the loop
        if (digitalRead(Reset_pin) == HIGH) {
          break;
        }

        delayMicroseconds(maxSpeed);
        digitalWrite(STEP_PIN_MOTOR_A, LOW);
        digitalWrite(STEP_PIN_MOTOR_E, LOW);
        delayMicroseconds(maxSpeed);
      }

      // Blink LED for laser simulation during scan
      for (int k = 0; k < 10; k++) {
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        if (digitalRead(Reset_pin) == HIGH) {
          digitalWrite(LED_PIN, LOW);
          break;
        }
        digitalWrite(LED_PIN, LOW);
        delay(50);
        if (digitalRead(Reset_pin) == HIGH) {
          break;
        }
      }
    }

    // Disable motor drivers
    digitalWrite(Enable_E_PIN, HIGH);
    digitalWrite(Enable_A_PIN, HIGH);

    // Check for reset button press to exit the loop
    if (digitalRead(Reset_pin) == HIGH) {
      break;
    }  
  }

  // Scan routine
  while (ScanStateOnce) {
    // Enable motor drivers
    digitalWrite(Enable_E_PIN, LOW);
    digitalWrite(Enable_A_PIN, LOW);
    delay(10);

    // Iterate through scan target positions
    for (int i = 0; i < sizeof(targetPositionsA_Scan) / sizeof(targetPositionsA_Scan[0]); i++) {
      long targetPositionMotorA = targetPositionsA_Scan[i];
      long targetPositionMotorE = targetPositionsE_Scan[i];

      // Set motor directions based on target positions
      if (targetPositionMotorA < currentPositionMotorA) {
        digitalWrite(DIR_PIN_MOTOR_A, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_A, HIGH);
      }
      if (targetPositionMotorE < currentPositionMotorE) {
        digitalWrite(DIR_PIN_MOTOR_E, LOW);
      } else {
        digitalWrite(DIR_PIN_MOTOR_E, HIGH);
      }

      // Check for reset button press to exit the loop
      if (digitalRead(Reset_pin) == HIGH) {
        break;
      }

      // Calculate steps to move for each motor
      long stepsToMoveMotorA = abs(targetPositionMotorA - currentPositionMotorA);
      long stepsToMoveMotorE = abs(targetPositionMotorE - currentPositionMotorE);

      // Move motors
      for (long j = 0; j < max(stepsToMoveMotorA, stepsToMoveMotorE); j++) {
        if (j < stepsToMoveMotorA) {
          digitalWrite(STEP_PIN_MOTOR_A, HIGH);
          currentPositionMotorA += (targetPositionMotorA < currentPositionMotorA) ? -1 : 1;
        }
        if (j < stepsToMoveMotorE) {
          digitalWrite(STEP_PIN_MOTOR_E, HIGH);
          currentPositionMotorE += (targetPositionMotorE < currentPositionMotorE) ? -1 : 1;
        }

        // Check for reset button press to exit the loop
        if (digitalRead(Reset_pin) == HIGH) {
          break;
        }

        delayMicroseconds(maxSpeed);
        digitalWrite(STEP_PIN_MOTOR_A, LOW);
        digitalWrite(STEP_PIN_MOTOR_E, LOW);
        delayMicroseconds(maxSpeed);
      }

      // Blink LED for laser simulation during scan
      for (int k = 0; k < 10; k++) {
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        if (digitalRead(Reset_pin) == HIGH) {
          digitalWrite(LED_PIN, LOW);
          break;
        }
        digitalWrite(LED_PIN, LOW);
        delay(50);
        if (digitalRead(Reset_pin) == HIGH) {
          break;
        }
      }
    }

    // Disable motor drivers
    digitalWrite(Enable_E_PIN, HIGH);
    digitalWrite(Enable_A_PIN, HIGH);

    // Check for reset button press to exit the loop
    if (digitalRead(Reset_pin) == HIGH) {
      break;
    }
  ScanStateOnce = 0; 
  return;    
  }

  // Reset routine
  if (resetPressed) {
    Serial.println("Reset button pressed. Moving motors to home position.");
    // Disable motor drivers
    digitalWrite(Enable_E_PIN, LOW);
    digitalWrite(Enable_A_PIN, LOW);

    // Set target positions for home position
    long targetPositionMotorA = 0;
    long targetPositionMotorE = 0;

    // Set motor directions based on currentpositions
    if (targetPositionMotorA < currentPositionMotorA) {
      digitalWrite(DIR_PIN_MOTOR_A, LOW);
    } else {
      digitalWrite(DIR_PIN_MOTOR_A, HIGH);
    }

    if (targetPositionMotorE < currentPositionMotorE) {
      digitalWrite(DIR_PIN_MOTOR_E, LOW);
    } else {
      digitalWrite(DIR_PIN_MOTOR_E, HIGH);
    }

    // Calculate steps to move for each motor
    long stepsToMoveMotorA = abs(targetPositionMotorA - currentPositionMotorA);
    long stepsToMoveMotorE = abs(targetPositionMotorE - currentPositionMotorE);

    // Move motors to home position
    for (long j = 0; j < (max(stepsToMoveMotorA, stepsToMoveMotorE) + 1); j++) {
      if (j < stepsToMoveMotorA) {
        digitalWrite(STEP_PIN_MOTOR_A, HIGH);
        currentPositionMotorA += (targetPositionMotorA < currentPositionMotorA) ? -1 : 1;
      }
      if (j < stepsToMoveMotorE) {
        digitalWrite(STEP_PIN_MOTOR_E, HIGH);
        currentPositionMotorE += (targetPositionMotorE < currentPositionMotorE) ? -1 : 1;
      }
      delayMicroseconds(maxSpeed);
      digitalWrite(STEP_PIN_MOTOR_A, LOW);
      digitalWrite(STEP_PIN_MOTOR_E, LOW);
      delayMicroseconds(maxSpeed);
    }

    // Reset current positions
    currentPositionMotorE = 0;
    currentPositionMotorA = 0;

    // Disable motor drivers
    digitalWrite(Enable_E_PIN, HIGH);
    digitalWrite(Enable_A_PIN, HIGH);

    resetPressed = false;
    Serial.println("Motors at home position.");
  }
}
