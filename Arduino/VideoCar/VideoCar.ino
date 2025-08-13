/*
  keyestudio ESP32-CAM Video Smart Car — 안정화 빌드
  - STA 모드(ipTIME 등 공유기 연결)
  - i2c_init() 비활성(카메라 SCCB와 충돌 방지)
  - 카메라 자원 보수 설정(해상도↓, xclk 10MHz, fb=1)
  - LED 깜빡임은 PWM(analogWrite) 대신 디지털 토글
*/

#include <WiFi.h>
#include "esp_wifi.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "SetMotor.h"   // 설치돼 있어야 함 (이 스케치에선 직접 사용하지 않음)
#include "app_server.h" // 아래 두 번째 파일

// ===== Wi-Fi (STA) =====
bool ap = 0; // 0: 공유기 연결(권장), 1: ESP32가 AP 열기
const char* ssid     = "PKNU_72";
const char* password = "iotiot72";

// ===== AP 모드 설정(미사용) =====
int channel       = 11;
int hidden        = 0;
int maxconnection = 1;

// ===== ESP32-CAM (AI-Thinker) 핀맵 =====
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#define LED_GPIO_NUM 12  // 키트 전방 라이트(플래시 GPIO4 아님)

// 웹서버 시작 함수(아래 헤더에 구현)
void startCameraServer();

void setup() {
  // 브라운아웃 경고 무시(전원 순간 강하시 리셋 방지) — 근본 해결은 안정 전원!
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();

  // (중요) 모터보드 I2C 초기화 끄기: 카메라 SCCB와 충돌 방지
  // i2c_init();  // <-- 사용 금지. 모터 필요해지면 I2C_NUM_1로 분리해서 다시 켜자.

  // 카메라 설정(보수)
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0; // 카메라 XCLK가 LEDC 사용 → 다른 PWM과 충돌 주의
  config.ledc_timer   = LEDC_TIMER_0;

  config.pin_d0   = Y2_GPIO_NUM;
  config.pin_d1   = Y3_GPIO_NUM;
  config.pin_d2   = Y4_GPIO_NUM;
  config.pin_d3   = Y5_GPIO_NUM;
  config.pin_d4   = Y6_GPIO_NUM;
  config.pin_d5   = Y7_GPIO_NUM;
  config.pin_d6   = Y8_GPIO_NUM;
  config.pin_d7   = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync= VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 10000000;          // 10MHz (안정 후 20MHz로 올려도 됨)
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size   = FRAMESIZE_QQVGA;  // 안정 후 QVGA로
    config.jpeg_quality = 18;               // 숫자 클수록 압축↑(부하↓)
    config.fb_count     = 1;                // 1부터
    config.fb_location  = CAMERA_FB_IN_PSRAM;
    config.grab_mode    = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size   = FRAMESIZE_QQVGA;
    config.jpeg_quality = 20;
    config.fb_count     = 1;
    config.grab_mode    = CAMERA_GRAB_LATEST;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }

  // 센서 추가 설정
  sensor_t* s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QQVGA);
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);

  // LED(라이트) 깜빡임: PWM(analogWrite) 쓰지 말고 토글
  pinMode(LED_GPIO_NUM, OUTPUT);
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_GPIO_NUM, LOW);
    delay(150);
    digitalWrite(LED_GPIO_NUM, HIGH);
    delay(150);
  }

  // 네트워크
  if (!ap) {
    Serial.println("ssid: " + (String)ssid);
    Serial.println("password: " + (String)password);
    Serial.println("WiFi is Client Scout32");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(400);
      Serial.print(".");
    }
    Serial.print("\nCamera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
  } else {
    Serial.println("WiFi is Standalone Scout32");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password, channel, hidden, maxconnection);
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());
    Serial.println("' to connect");
  }

  // 웹서버 시작
  startCameraServer();
}

void loop() {
  delay(1000);
  // Serial.printf("RSSI: %ld dBm\n", WiFi.RSSI());
}