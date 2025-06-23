# 06.23 김준태 어플 구동 테스트 용으로 만든 
# 플라스크 서버

# 가상환경 진입법
# python -m venv venv
# venv/Scripts/activate
# 가상환경 진입하고 나서
# pip install -r requirements.txt
# 패키지 새로 설치하는 게 있으면 pip freeze > requirements.txt
# 가상환경 종료할 일 있으면 deactivate

# 라고 지피티가 그랬어

# auth.py
from flask import Blueprint, request, jsonify
from db import db, conn
import bcrypt # 비번 암호화
import uuid # qr코드 생성

auth = Blueprint('auth', __name__)

@auth.route('/register', methods=['POST'])
def register():
    data = request.json
    email = data['email']
    pw = data['password'].encode('utf-8')

    qr_token = str(uuid.uuid4()) # 고유 QR코드 생성
    hashed = bcrypt.hashpw(pw, bcrypt.gensalt())

    sql = "INSERT INTO member (email, password, qrcode) VALUES (%s, %s, %s)"
    db.execute(sql, (email, hashed.decode('utf-8'), qr_token))
    conn.commit()

    return jsonify({"message": "회원가입 성공!"}), 201

@auth.route('/login', methods=['POST'])
def login():
    data = request.json
    email = data['email']
    pw = data['password'].encode('utf-8')

    db.execute("SELECT * FROM member WHERE email = %s", (email,))
    user = db.fetchone()

    if user and bcrypt.checkpw(pw, user['password'].encode('utf-8')):
        return jsonify({"message": "로그인 성공!",
                        "qr_token": user['qrcode']})
    else:
        return jsonify({"message": "로그인 실패"}), 401
