#ifndef APP_SERVER_H
#define APP_SERVER_H

// ---- User toggles ----
static const bool MOTOR_ENABLED = false;  // 모터 I2C 안 쓸 거면 false 유지

// Define Servo PWM Values & Stopped Variable
int speed = 100;
int trim  = 0;

// Libraries
#include <esp32-hal-ledc.h>
#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"
// #include "lib/dl_lib.h"  // (삭제) dl_matrix3du_* 안 씀 → 링크 에러 방지
#include "SetMotor.h"

// Stream Encoding
typedef struct {
  httpd_req_t *req;
  size_t len;
} jpg_chunking_t;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY     = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART         = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
httpd_handle_t stream_httpd = NULL;
httpd_handle_t camera_httpd = NULL;

static size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len) {
  jpg_chunking_t *j = (jpg_chunking_t *)arg;
  if (!index) j->len = 0;
  if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) return 0;
  j->len += len;
  return len;
}

// (단순화) dl_matrix3du_* 제거 — JPEG이면 그대로, 아니면 frame2jpg_cb
static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");

  esp_err_t res = ESP_OK;
  int64_t fr_start = esp_timer_get_time();

  if (fb->format == PIXFORMAT_JPEG) {
    res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
  } else {
    jpg_chunking_t jchunk = { req, 0 };
    bool ok = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk);
    esp_camera_fb_return(fb);
    if (!ok) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    httpd_resp_send_chunk(req, NULL, 0);
  }

  int64_t fr_end = esp_timer_get_time();
  Serial.printf("JPG: %ums\n", (uint32_t)((fr_end - fr_start) / 1000));
  return res;
}

static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t *_jpg_buf = NULL;
  char part_buf[64];  // (수정) char *part_buf[64] → char part_buf[64]

  static int64_t last_frame = 0;
  if (!last_frame) last_frame = esp_timer_get_time();

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) return res;

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if (fb->format != PIXFORMAT_JPEG) {
        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
        esp_camera_fb_return(fb);
        fb = NULL;
        if (!jpeg_converted) {
          Serial.println("JPEG compression failed");
          res = ESP_FAIL;
        }
      } else {
        _jpg_buf_len = fb->len;
        _jpg_buf     = fb->buf;
      }
    }

    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, sizeof(part_buf), _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));

    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }

    if (res != ESP_OK) break;

    int64_t fr_end = esp_timer_get_time();
    int64_t frame_time = (fr_end - last_frame) / 1000;
    last_frame = fr_end;
    if (frame_time == 0) frame_time = 1;
    Serial.printf("MJPG: %uB %ums (%.1ffps)\n",
                  (uint32_t)(_jpg_buf_len),
                  (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time);
  }

  last_frame = 0;
  return res;
}

// Control Handling from Server
enum state { fwd, rev, stp };
state actstate = stp;

static esp_err_t cmd_handler(httpd_req_t *req) {
  char *buf;
  size_t buf_len;
  char variable[32] = {0};
  char value[32]    = {0};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char *)malloc(buf_len);
    if (!buf) { httpd_resp_send_500(req); return ESP_FAIL; }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (!(httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
            httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK)) {
        free(buf); httpd_resp_send_404(req); return ESP_FAIL;
      }
    } else { free(buf); httpd_resp_send_404(req); return ESP_FAIL; }
    free(buf);
  } else { httpd_resp_send_404(req); return ESP_FAIL; }

  int val = atoi(value);
  sensor_t *s = esp_camera_sensor_get();
  int res = 0;

  if (!strcmp(variable, "framesize")) {
    if (s->pixformat == PIXFORMAT_JPEG) res = s->set_framesize(s, (framesize_t)val);
  } else if (!strcmp(variable, "quality")) {
    res = s->set_quality(s, val);
  }
  // --- Lights / Flash ---
  else if (!strcmp(variable, "flash")) {
    // PWM(analogWrite) 금지 — 카메라 LEDC와 충돌. 단순 on/off.
    pinMode(12, OUTPUT);
    digitalWrite(12, (val > 0) ? HIGH : LOW);
    res = 0;
  }
  // --- Car / Motor ---
  else if (!strcmp(variable, "car")) {
    if (!MOTOR_ENABLED) {
      // 모터 비활성 시 아무것도 안 함 (I2C 호출 금지)
      actstate = stp;
      res = 0;
    } else {
      if (val == 1) { // Forward
        int speed1 = max(0, min(255, speed - trim));
        int speed2 = max(0, min(255, speed + trim));
        actstate = fwd;
        i2c_Write(M1_PWM, speed1); i2c_Write(M1_IN, 0);
        i2c_Write(M2_PWM, 0);      i2c_Write(M2_IN, speed2);
      } else if (val == 2) { // Backward
        int speed1 = max(0, min(255, speed - trim));
        int speed2 = max(0, min(255, speed + trim));
        actstate = rev;
        i2c_Write(M1_PWM, 0);      i2c_Write(M1_IN, speed1);
        i2c_Write(M2_PWM, speed2); i2c_Write(M2_IN, 0);
      } else if (val == 3) { // Left
        i2c_Write(M1_PWM, 150); i2c_Write(M1_IN, 0);
        i2c_Write(M2_PWM, 150); i2c_Write(M2_IN, 0);
      } else if (val == 4) { // Right
        i2c_Write(M1_PWM, 0);   i2c_Write(M1_IN, 150);
        i2c_Write(M2_PWM, 0);   i2c_Write(M2_IN, 150);
      } else if (val == 5) { // Stop
        Car_stop();
        actstate = stp;
      }
    }
  } else if (!strcmp(variable, "speed")) {
    if (val > 8) val = 8; else if (val < 0) val = 0;
    speed = val * 31;
  } else if (!strcmp(variable, "trim")) {
    if (val > 32) val = 32; else if (val < -32) val = -32;
    trim = val;
  } else {
    res = -1;
  }

  if (res) return httpd_resp_send_500(req);
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static esp_err_t status_handler(httpd_req_t *req) {
  static char json_response[256];
  sensor_t *s = esp_camera_sensor_get();
  int n = snprintf(json_response, sizeof(json_response),
                   "{\"framesize\":%u,\"quality\":%u}",
                   s->status.framesize, s->status.quality);
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json_response, n);
}

// ---- Front-End HTML (원본 유지, 아이콘 base64만 짧게) ----
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=400px,user-scalable=no">
    <title>Keyestudio ESP32-CAM Video Car</title>
    <link href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAA..." rel="icon" />
    <style>
      body{width:100%;padding:0;margin:0;overflow-x:hidden;line-height:0;font-family:sans-serif;background:#FFF;color:#000;font-size:14px}
      section table{width:400px;margin:auto;padding:0;}
      section img{width:400px;margin:auto;display:block;padding:0;}
      figure{margin:0;padding:0;}
      tr{margin:0;}
      button{display:block;width:128px;height:64px;margin:2px 0;border:0;color:#fff;background:#734CA7;border-radius:8px;outline:0}
      button{background:linear-gradient(to bottom,#856fa5 0%,#734ca7 50%,#673ba5 51%,#7f63a5 100%);box-shadow:0 2px 1px #15052D;}
      button{-webkit-touch-callout:none;-webkit-user-select:none;-khtml-user-select:none;-moz-user-select:none;user-select:none;}
      button.lr{height:136px;}
      input[type=range]{-webkit-appearance:none;width:100%;height:24px;background:#363636;margin:2px 0;}
      input[type=range]:focus{outline:0}
      input[type=range]::-webkit-slider-runnable-track{width:100%;height:4px;cursor:pointer;background:#EFEFEF;border-radius:0;border:0 solid #EFEFEF}
      input[type=range]::-webkit-slider-thumb{border:1px solid rgba(0,0,30,0);height:32px;width:32px;border-radius:50px;background:#734CA7;cursor:pointer;-webkit-appearance:none;margin-top:-14px}
      input[type=range]:focus::-webkit-slider-runnable-track{background:#EFEFEF}
      input[type=range]::-moz-range-track{width:100%;height:4px;background:#EFEFEF;border-radius:0;border:0 solid #EFEFEF}
      input[type=range]::-moz-range-thumb{border:1px solid rgba(0,0,30,0);height:32px;width:32px;border-radius:50px;background:#734CA7;cursor:pointer}
    </style>
  </head>
  <body>
    <section id="main">
      <figure>
        <div id="stream-container" class="image-container">
          <img width="400px" height="300px" id="stream" src="">
        </div>
      </figure>
      <section id="buttons">
        <table>
          <tr>
            <td align="center"><button id="forward" onpointerdown="document.dispatchEvent(fwdpress);" onpointerup="document.dispatchEvent(fwdrelease);" onpointerleave="document.dispatchEvent(fwdrelease);">Forward</button></td>
            <td align="center" rowspan="2"><button class="lr" id="turnleft" onpointerdown="document.dispatchEvent(leftpress);" onpointerup="document.dispatchEvent(leftrelease);" onpointerleave="document.dispatchEvent(leftrelease);">Left</button></td>
            <td align="center" rowspan="2"><button class="lr" id="turnright" onpointerdown="document.dispatchEvent(rightpress);" onpointerup="document.dispatchEvent(rightrelease);" onpointerleave="document.dispatchEvent(rightrelease);">Right</button></td>
          </tr>
          <tr>
            <td align="center"><button id="backward" onpointerdown="document.dispatchEvent(backpress);" onpointerup="document.dispatchEvent(backrelease);" onpointerleave="document.dispatchEvent(backrelease);">Backward</button></td>
          </tr>
          <tr><td align="center">Speed</td><td align="center" colspan="2"><input type="range" id="speed" min="0" max="8" value="8" onchange="try{fetch(document.location.origin+'/control?var=speed&val='+this.value);}catch(e){}"></td></tr>
          <tr><td align="center">Trim</td><td align="center" colspan="2"><input type="range" id="speed" min="-32" max="32" value="0" onchange="try{fetch(document.location.origin+'/control?var=trim&val='+this.value);}catch(e){}"></td></tr>
          <tr><td align="center">Lights</td><td align="center" colspan="2"><input type="range" id="flash" min="0" max="255" value="10" onchange="try{fetch(document.location.origin+'/control?var=flash&val='+this.value);}catch(e){}"></td></tr>
          <tr><td align="center">Quality</td><td align="center" colspan="2"><input type="range" id="quality" min="10" max="63" value="10" onchange="try{fetch(document.location.origin+'/control?var=quality&val='+this.value);}catch(e){}"></td></tr>
          <tr><td align="center">Resolution</td><td align="center" colspan="2"><input type="range" id="framesize" min="0" max="6" value="5" onchange="try{fetch(document.location.origin+'/control?var=framesize&val='+this.value);}catch(e){}"></td></tr>
        </table>
      </section>
    </section>

    <script>
      // 스트림 시작
      var source = document.getElementById('stream');
      source.src = document.location.origin+':81/stream';

      // 키/터치 제어
      var keyforward=0, keybackward=0, keyleft=0, keyright=0;
      var fwdpress = new KeyboardEvent('keydown', {'keyCode':38, 'which':38});
      var fwdrelease = new KeyboardEvent('keyup',   {'keyCode':38, 'which':38});
      var backpress = new KeyboardEvent('keydown', {'keyCode':40, 'which':40});
      var backrelease = new KeyboardEvent('keyup', {'keyCode':40, 'which':40});
      var leftpress = new KeyboardEvent('keydown', {'keyCode':37, 'which':37});
      var leftrelease = new KeyboardEvent('keyup', {'keyCode':37, 'which':37});
      var rightpress = new KeyboardEvent('keydown', {'keyCode':39, 'which':39});
      var rightrelease = new KeyboardEvent('keyup', {'keyCode':39, 'which':39});

      document.addEventListener('keydown',function(e){
        e.preventDefault();
        if ((e.keyCode==38)&&(!keybackward)&&(!keyforward)) keyforward=1;
        else if ((e.keyCode==40)&&(!keyforward)&&(!keybackward)) keybackward=1;
        else if ((e.keyCode==37)&&(!keyright)&&(!keyleft)) keyleft=1;
        else if ((e.keyCode==39)&&(!keyleft)&&(!keyright)) keyright=1;
      });
      document.addEventListener('keyup',function(e){
        if ((e.keyCode==38)||(e.keyCode==40)) {keyforward=0;keybackward=0;}
        else if ((e.keyCode==37)||(e.keyCode==39)) {keyleft=0;keyright=0;}
      });

      var currentcommand=0, oldcommand=0;
      window.setInterval(function(){
        if (((keyforward)&&(keyleft)) || ((keybackward)&&(keyleft)) || (keyleft)) currentcommand=3;
        else if (((keyforward)&&(keyright)) || ((keybackward)&&(keyright)) || (keyright)) currentcommand=4;
        else if (keyforward) currentcommand=1;
        else if (keybackward) currentcommand=2;
        else currentcommand=5;
        if (currentcommand!=oldcommand){
          fetch(document.location.origin+'/control?var=car&val='+currentcommand);
          oldcommand=currentcommand;
        }
      }, 100);
    </script>
  </body>
</html>
)rawliteral";

static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  httpd_uri_t index_uri   = { .uri="/",        .method=HTTP_GET, .handler=index_handler,   .user_ctx=NULL };
  httpd_uri_t status_uri  = { .uri="/status",  .method=HTTP_GET, .handler=status_handler,  .user_ctx=NULL };
  httpd_uri_t cmd_uri     = { .uri="/control", .method=HTTP_GET, .handler=cmd_handler,     .user_ctx=NULL };
  httpd_uri_t capture_uri = { .uri="/capture", .method=HTTP_GET, .handler=capture_handler, .user_ctx=NULL };
  httpd_uri_t stream_uri  = { .uri="/stream",  .method=HTTP_GET, .handler=stream_handler,  .user_ctx=NULL };

  Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    httpd_register_uri_handler(camera_httpd, &status_uri);
    httpd_register_uri_handler(camera_httpd, &capture_uri);
  }

  config.server_port += 1;
  config.ctrl_port   += 1;
  Serial.printf("Starting stream server on port: '%d'\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

#endif