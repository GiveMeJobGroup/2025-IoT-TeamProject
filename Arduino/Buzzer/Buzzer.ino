const int buzzerPin = D2;

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(buzzerPin, HIGH); // 부저 ON
  delay(500);
  digitalWrite(buzzerPin, LOW);  // 부저 OFF
  delay(500);
}