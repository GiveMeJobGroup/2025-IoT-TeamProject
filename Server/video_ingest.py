from flask import Blueprint, Response, request, jsonify
import cv2
import requests

bp = Blueprint("video", __name__)

ESP_HOST = "http://192.168.0.12"   # ESP32 IP
STREAM_URL = f"{ESP_HOST}/stream"
MOTOR_URL  = f"{ESP_HOST}/motor"

# 간단 테스트: /video/show 로 들어오면 OpenCV 창에 띄움(개발용)
@bp.route("/show", methods=["GET"])
def show():
    cap = cv2.VideoCapture(STREAM_URL)
    if not cap.isOpened():
        return "fail open stream", 500

    while True:
        ok, frame = cap.read()
        if not ok:
            break

        # --- 여기서 라인트레이싱 판단(임시: 화면 가운데보다 왼/오 구분만) ---
        h, w = frame.shape[:2]
        cx = w // 2
        # 예시: 왼쪽 밝기 합 vs 오른쪽 밝기 합 비교해서 조향
        left_sum  = frame[:, :cx].mean()
        right_sum = frame[:, cx:].mean()
        steer = (right_sum - left_sum)  # 대충 오른쪽이 밝으면 오른쪽으로

        base = 100
        k = 0.1
        l = int(max(0, min(255, base - k * steer)))
        r = int(max(0, min(255, base + k * steer)))
        try:
            requests.get(MOTOR_URL, params={"l": l, "r": r}, timeout=0.1)
        except Exception:
            pass

        cv2.imshow("ESP32", frame)
        if cv2.waitKey(1) & 0xFF == 27:  # ESC
            break

    cap.release()
    cv2.destroyAllWindows()
    return "ok"

# 실제 서비스에서는 /video/drive 같은 POST로 서버가 계산해서 모터 명령만 보냄
@bp.route("/drive", methods=["POST"])
def drive():
    l = int(request.json.get("l", 0))
    r = int(request.json.get("r", 0))
    try:
        requests.get(MOTOR_URL, params={"l": l, "r": r}, timeout=0.3)
        return jsonify(ok=True)
    except Exception as e:
        return jsonify(ok=False, error=str(e)), 500