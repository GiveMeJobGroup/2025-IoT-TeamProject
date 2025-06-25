#include <Servo.h>

Servo myservo1;
Servo myservo2;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup 시작");

  myservo1.attach(4); // D2
  myservo2.attach(5); // D1

  myservo1.write(90);
  myservo2.write(90);
  Serial.println("모두 90도 위치로 초기화");
  delay(2000);
}

void loop() {
  Serial.println("myservo1: 0도 이동");
  myservo1.write(0);
  delay(2000);

  Serial.println("myservo1: 180도 이동");
  myservo1.write(180);
  delay(2000);

  Serial.println("myservo2: 0도 이동");
  myservo2.write(0);
  delay(2000);

  Serial.println("myservo2: 180도 이동");
  myservo2.write(180);
  delay(2000);
}