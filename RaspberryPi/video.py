from picamera2 import Picamera2
import cv2, requests, time

SERVER = "http://192.168.0.3:5000/video/ingest"  # ← 서버 IP로 교체
QUALITY = 70                                   # 50~80 적당

picam2 = Picamera2()
cfg = picam2.create_video_configuration(
    main={"size": (640, 480), "format": "RGB888"},
    controls={"FrameDurationLimits": (33333, 33333)}  # ≈30fps
)
picam2.configure(cfg)
picam2.start()
time.sleep(0.3)

s = requests.Session()
n = 0
while True:
    frame = picam2.capture_array()
    ok, jpg = cv2.imencode(".jpg", frame, [int(cv2.IMWRITE_JPEG_QUALITY), QUALITY])
    if not ok: continue
    try:
        r = s.post(SERVER, files={"frame": ("f.jpg", jpg.tobytes(), "image/jpeg")}, timeout=2)
        n += 1
        if n % 30 == 0:
            print("POST", n, r.status_code)
    except requests.RequestException as e:
        print("POST FAIL:", e)
        time.sleep(0.5)