#include <ESP8266WiFi.h>            // ESP8266 라이브러리
#include <ESP8266HTTPClient.h>      // ESP8266 <-> 서버 통신 라이브러리
#include <ArduinoJson.h>            // JSON 형태 사용 라이브러리

#include <SPI.h>
#include <MFRC522.h>  // RFID 리더기

const char* ssid = "rbk";                // Wi-Fi SSID
const char* password = "57875787";          // Wi-Fi 비밀번호
const char* serverUrl_send = "http://210.119.12.72:5000/rfid";  // 센서 데이터 서버에 전송
const char* serverUrl_get = "http://210.119.12.72:5000/get_data";      // 서버에서 데이터 수신

constexpr uint8_t RST_PIN = D3; 
constexpr uint8_t SS_PIN = D4;  

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;


// 초기화 함수
void setup() {   
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  // 와이파이 연결 확인
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("WiFi 연결 중...");
  }
  Serial.println("WiFi 연결 성공!");

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void loop(){
  if (read_RFID()) {
  sendTagToServer(tag);
  tag = "";  // 다음 태그를 위해 초기화
  }

}

bool read_RFID(){
  if ( ! rfid.PICC_IsNewCardPresent())
    return false;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return true;
  }
  return false;
}

void sendTagToServer(String tagValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;  // ✅ 추가
    http.begin(client, serverUrl_send);  // ✅ 수정

    http.addHeader("Content-Type", "application/json");
    String payload = "{\"rfid\":\"" + tagValue + "\"}";
    int httpCode = http.POST(payload);

    if (httpCode == 200) {
      String response = http.getString();
      Serial.println("서버 응답: " + response);

      DynamicJsonDocument doc(256);
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        bool exists = doc["exists"];
        if (exists) {
          Serial.println("📚 대출된 도서입니다.");
        } else {
          Serial.println("📕 대출되지 않은 도서입니다.");
        }
      } else {
        Serial.println("JSON 파싱 실패");
      }
    } else {
      Serial.print("서버 오류 코드: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi 연결 안 됨");
  }
}