/*
  Turns an LED on for one second, then off for one second, repeatedly.
*/

const int ledPin = 6;       // LED

// the setup function runs once when you press reset or power the board

void setup() {
  // initialize digital pin ledpin as an output.
  pinMode(ledpin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ledpin, HIGH);  
  delay(1000);                     
  digitalWrite(ledpin, LOW);  
  delay(1000);                     
}
