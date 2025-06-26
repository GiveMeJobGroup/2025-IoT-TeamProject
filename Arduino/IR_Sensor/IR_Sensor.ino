int led = 2;      // GPIO2 = 내장 LED (D4)
int sensor = 0;   // GPIO5 = D1

void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
}

void loop(){
  int data = digitalRead(sensor);
  if (data == HIGH){
    digitalWrite(led, LOW);      // LED OFF
    Serial.println("물체가 인식되지 않습니다.");
    delay(1000);
  }
  else {
    digitalWrite(led, HIGH);     // LED ON
    Serial.println("물체가 인식되었습니다.");
    delay(1000);
  }
}