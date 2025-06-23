# db.py
import mysql.connector
import pymysql

host = 'localhost'
database = 'Library'
username = 'root'
password = '12345'

try:
    conn = pymysql.connect(
        host=host,
        user=username,
        passwd=password,
        db=database,
        cursorclass=pymysql.cursors.DictCursor  # 데이터베이스 쿼리 결과를 딕셔너리 형태로 가져오도록 설정
    )
    cursor = conn.cursor()
    print("MySQL 연결 성공")

except pymysql.MySQLError as e:
    print(f"MySQL 오류 발생: {e}")
    conn = None