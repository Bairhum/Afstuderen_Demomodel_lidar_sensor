

// Pin assignments for buttons
#define Dance_pin  2
#define Reset_pin  3
#define Scan_pin  4

// Pin assignments for LED
#define LED_PIN 13


void setup() {
  pinMode(Scan_pin, INPUT);
  pinMode(Reset_pin, INPUT);
  pinMode(Dance_pin, INPUT);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {
 if (Scan_pin || Reset_pin || Dance_pin) {
    // turn LED on:
    digitalWrite(LED_PIN, HIGH);
  } else {
    // turn LED off:
    digitalWrite(LED_PIN, LOW);
  }
}
