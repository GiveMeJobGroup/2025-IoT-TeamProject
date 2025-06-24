// 3번과 4번 핀은 모터 제어에 관한 핀
int led = 2;      // GPIO2 = 내장 LED (D4)
int sensor = 5;   // GPIO5 = D1

int IN1Pin = 14;
int IN2Pin = 12;

// 5번핀은 모터의 힘을 설정해주는 핀
int ENPin = 13;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT); // 3, 4번 제어핀들은 핀모드를 출력은로 설정
  analogWrite(ENPin, 190); //5번 핀에 255의 최대 힘을 설정한다. 
}

void loop() {    
  int data = digitalRead(sensor);
  if (data == HIGH){
    digitalWrite(led, LOW);      // LED OFF
    Serial.println("물체가 인식되지 않습니다.");
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, LOW);
    delay(1000);
  }
  else {
    digitalWrite(led, HIGH);     // LED ON
    Serial.println("물체가 인식되었습니다.");
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);  
    delay(5000);
  }
}