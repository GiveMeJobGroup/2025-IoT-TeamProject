#include <Servo.h>

Servo myservo1;
Servo myservo2;

// 핀 설정
const int led = D4;          // 내장 LED (GPIO2)
const int sensor = 0;       // 적외선 센서 (GPIO16)

const int IN1Pin = D6;       // DC모터 제어핀 1 (GPIO12)
const int IN2Pin = D7;       // DC모터 제어핀 2 (GPIO13)
const int ENPinA = D1;       // DC모터 속도 제어 (PWM 가능, GPIO14)

void setup() {
  Serial.begin(115200);
  Serial.println("Setup 시작");

  // 서보모터 연결
  myservo1.attach(D2);       // D2로 변경하려면 여기 수정 필요 (아래 설명 참조)
  myservo1.write(0);          // 0도 초기화
  myservo2.attach(D5);
  myservo2.write(0);
  Serial.println("서보모터 0도로 초기화");
  delay(1000);

  // 핀모드 설정
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);

  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(ENPinA, OUTPUT);

  analogWrite(ENPinA, 190); // DC모터 힘 설정 (0~255)

  Serial.println("모든 장치 초기화 완료");
}

void loop() {
  int data = digitalRead(sensor);

  if (data == HIGH) {
    // 아무것도 없음
    digitalWrite(led, LOW); // LED 꺼짐
    Serial.println("물체 없음");

    // DC 모터 정지
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, LOW);

    // 서보모터 0도로 복귀
    myservo1.write(0);
    myservo2.write(0);
    delay(1000);
  }
  else {
    // 물체 감지됨
    digitalWrite(led, HIGH);  // LED 켜짐
    Serial.println("물체 감지됨!");

    // DC모터 회전 (전진 방향)
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);

    // 서보모터 30도 이동
    myservo1.write(70);
    myservo2.write(70);
    Serial.println("서보모터 30도 이동");

    delay(5000); // 5초 유지
  }
}


// void setup(){}
// void loop(){}