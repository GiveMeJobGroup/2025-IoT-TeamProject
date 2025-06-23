import pymysql
import random
import requests
from flask import Blueprint, request, jsonify, render_template
from db import cursor, conn
import bcrypt

RFID_data = Blueprint('RFID_data', __name__)
@RFID_data.route('/')
def index():
    return render_template('data.html')

@RFID_data.route('/rfid', methods=['POST'])
def check_rfid():
    try:
        req = request.json
        rfid = req.get('rfid')

        if not rfid:
            return jsonify({"message": "RFID 값이 없습니다"}), 400

        sql = "SELECT COUNT(*) AS cnt FROM Book WHERE Rfid = %s"
        cursor.execute(sql, (rfid,))
        result = cursor.fetchone()

        if result['cnt'] == 0:
            return jsonify({"message": "등록되지 않은 카드", "exists": False}), 200
        else:
            return jsonify({"message": "등록된 카드", "exists": True}), 200

    except Exception as e:
        return jsonify({"message": "서버 오류", "error": str(e)}), 500