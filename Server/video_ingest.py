from flask import Blueprint, request, Response, jsonify
import cv2, numpy as np, time

bp = Blueprint("video", __name__)
_last_jpeg = None
_last_err  = None   # -1 ~ +1 (좌/우 오프셋)

def detect_line(img):
    h, w = img.shape[:2]
    y0 = int(h*0.55)             # 하단 ROI
    roi = img[y0:h, :]

    g = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
    _, bw = cv2.threshold(g, 120, 255, cv2.THRESH_BINARY_INV)  # 검정 라인 가정
    bw = cv2.medianBlur(bw, 5)

    cnts, _ = cv2.findContours(bw, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    vis = img.copy(); err = None
    if cnts:
        c = max(cnts, key=cv2.contourArea)
        M = cv2.moments(c)
        if M["m00"] > 0:
            cx = int(M["m10"]/M["m00"])
            err = (cx - w/2)/(w/2)                    # -1 ~ +1
            cv2.drawContours(roi, [c], -1, (255,0,0), 2)
            cv2.circle(roi, (cx, roi.shape[0]//2), 6, (0,255,0), -1)
    vis[y0:h, :] = roi
    txt = "NO LINE" if err is None else f"ERR={err:+.2f}"
    cv2.putText(vis, txt, (10,30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,0), 2)
    return vis, err

@bp.post("/ingest")
def ingest():
    global _last_jpeg, _last_err
    f = request.files.get("frame")
    if not f: return "no frame", 400
    img = cv2.imdecode(np.frombuffer(f.read(), np.uint8), cv2.IMREAD_COLOR)
    if img is None: return "bad jpeg", 400

    vis, err = detect_line(img)
    _last_err = err
    ok, jpg = cv2.imencode(".jpg", vis)
    if ok: _last_jpeg = jpg.tobytes()
    return "OK", 200

@bp.get("/stream")
def stream():
    def gen():
        global _last_jpeg
        if _last_jpeg is None:
            blank = np.zeros((240,320,3), np.uint8)
            cv2.putText(blank,"waiting...",(60,120),0,0.8,(255,255,255),2)
            _last_jpeg = cv2.imencode(".jpg", blank)[1].tobytes()
        while True:
            yield (b"--frame\r\nContent-Type: image/jpeg\r\n\r\n" + _last_jpeg + b"\r\n")
            time.sleep(0.03)
    return Response(gen(), mimetype="multipart/x-mixed-replace; boundary=frame")

@bp.get("/status")
def status():
    return jsonify({"err": _last_err})