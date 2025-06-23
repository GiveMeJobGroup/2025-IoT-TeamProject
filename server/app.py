# app.py
from flask import Flask
from flask_cors import CORS
from auth import auth

app = Flask(__name__)
CORS(app)  # CORS 허용 (Expo에서 접근 가능하도록)

app.register_blueprint(auth)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
