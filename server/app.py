# app.py
from flask import Flask
from flask_cors import CORS
# from auth import auth
# from RFID_data import RFID_data
from video_ingest import bp as video_bp

app = Flask(__name__)
CORS(app)  # CORS 허용 (Expo에서 접근 가능하도록)

# app.register_blueprint(auth)
# app.register_blueprint(RFID_data)
app.register_blueprint(video_bp, url_prefix="/video")

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, threaded=True)
